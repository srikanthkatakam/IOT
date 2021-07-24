import RPi.GPIO as GPIO
import cv2
import sys
import logging as log
import datetime as dt
from time import sleep
import os
import MySQLdb as mdb
import boto3
global a
from pyfingerprint.pyfingerprint import PyFingerprint

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(2, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #TRS
GPIO.setup(3, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)  #TDP
GPIO.setup(14, GPIO.OUT)
GPIO.setup(15, GPIO.OUT)
GPIO.output(14, False)
GPIO.output(15, False)



try:
    f = PyFingerprint('/dev/ttyUSB0', 9600, 0xFFFFFFFF, 0x00000000)

    if ( f.verifyPassword() == False ):
        raise ValueError('The given fingerprint sensor password is wrong!')

except Exception as e:
    print('The fingerprint sensor could not be initialized!')
    print('Exception message: ' + str(e))
    
print('Currently used templates: ' + str(f.getTemplateCount()) +'/'+ str(f.getStorageCapacity()))

s3_bucket = 'evmbucket1'
s3_bucket_region = 'us-east-1'

s3 = boto3.resource('s3')
my_bucket = s3.Bucket(s3_bucket)


entry_folder_name = "entry/"
exit_folder_name = "exit/"

q = []
############################# function for listing faces in folders of a bucket
entry_folder = "entry/"
exit_folder = "exit/"
a = []
b = []
images = []
c=0
###############################################################

faceCascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
log.basicConfig(filename='webcam.log',level=log.INFO)

video_capture = cv2.VideoCapture(0)
anterior = 0
i=0
offset=10
name=str (dt.datetime.now())


folder = 'entry'
s3 = boto3.client('s3')
key_name = folder + '/' 
s3_connect = boto3.client('s3', s3_bucket_region)

try:
    bucket = s3_connect.put_object(Bucket=s3_bucket, Key=key_name)
    #print ("Bucket:", bucket)
except Exception as e:
    print ("Bucket Error " , e)

 # i eddited not to get ret=false  ................................................final haarcascade_frontalface_default.xml'



while(not video_capture.isOpened()):    # i eddited not to get ret=false
    video_capture = cv2.VideoCapture(0)

ret = video_capture.isOpened()
print (ret)

def list_faces():
    c=0
   
    del images[:]
    
    for file in my_bucket.objects.all():
        g = file.key
        print (".......after:" + str(g))
        images.append(g)


    
    for x in images:
        if( x == entry_folder ):
            continue

        elif(x != exit_folder):
            a.append(x)
           
        if(x == exit_folder):
            break

    for y in  images:
        if(y != exit_folder):
            if(c==1):
                b.append(y)
                
            else:
                continue

        elif(y == exit_folder):
            c=1

        else:
            break

################################delete object

def delete_faces(bucket, key, region="us-east-1"):
                        rekognition = boto3.client("rekognition", region)
                        objs = list(my_bucket.objects.filter(Prefix=key))
                        response = my_bucket.delete_objects(
                            Delete={
                                'Objects': [
                                    {
                                        'Key': key,
                                     
                                        },
                                    ],
                                'Quiet': True|False
                                },
                            )
                        return response

############################## face comparision
def compare_faces(bucket, key, bucket_target, key_target, threshold=0, region="us-east-1"):
    rekognition = boto3.client("rekognition", region)
    response = rekognition.compare_faces(
        SourceImage={
            "S3Object": {
                "Bucket": bucket,
                "Name": key,
                }
            },
        TargetImage={
            "S3Object": {
                "Bucket": bucket_target,
                "Name": key_target,
                }
            },
        SimilarityThreshold=threshold,
        )
    return response['SourceImageFace'], response['FaceMatches']

def compare():
    list_faces()
    print(a,b,q)
    for i in a:
        for j in b:
            KEY_SOURCE = i
            KEY_TARGET = j


            print ("............face comparision started.....................")
            print (i,j)

            source_face, matches = compare_faces(s3_bucket, KEY_SOURCE, s3_bucket, KEY_TARGET)


            print ("Source Face ({Confidence}%)".format(**source_face))


            for match in matches:
                    print ("Target Face ({Confidence}%)".format(**match['Face']))
                    percent = match['Similarity']
                            
                    print ("  Similarity : {}%".format(match['Similarity']))
                    print("......................face comparision ended ............................................")

                    print(percent)
                    if(percent>80):
                        if i not in q:
                            q.append(i)
    print (q)
    b.clear()
    a.clear()
    if not q:
        print('unauthorized')
    else:
        verification()
    
    for key in q:
        delete_faces(s3_bucket, key, region="us-east-1")
        q.clear()

            
def insertDB(positionNumber,voting, Datetime):
    try:
        con=mdb.connect('localhost','root','123456789','evm');
        cursor = con.cursor()
        sql = "INSERT INTO fingerprint(positionNumber,voting, Datetime)\
                VALUES ('%s','%s','%s')"%\
                (positionNumber,voting, Datetime)
        cursor.execute(sql)
        print ('Inserted')
        sql = []
        con.commit()
        con.close()
    except:
        sleep(1)
        print('Not Inserted')  

def verification():
    print('Authorized')
    Datetime=name
    global voting    
    try:
        print('Waiting for finger...')

        ## Wait that finger is read
        while ( f.readImage() == False ):
            pass
            

        ## Converts read image to characteristics and stores it in charbuffer 1
        f.convertImage(0x01)

        ## Searchs template
        result = f.searchTemplate()

        positionNumber = result[0]
        accuracyScore = result[1]

        if ( positionNumber == -1 ):
            print('No match found!')
        else:
            
            print('Found template at position #' + str(positionNumber))
            print('The accuracy score is: ' + str(accuracyScore))
            sleep(8)
            TRS=GPIO.input(2)
            TDP=GPIO.input(3)
            print(TRS)
            print(TDP)
            print('Hello')
            
            if (TRS==0):
                voting='TRS'
                print('KCR')
                GPIO.output(14, True)
                sleep(1)
                insertDB(positionNumber,voting, Datetime)
                
            elif (TRS==1):
                voting='SB'
                GPIO.output(14, False)
                print('else1')
                
            if (TDP==0):
                voting='TDP'
                print('Chandrababu Naidu')
                GPIO.output(15, True)
                sleep(1)
                insertDB(positionNumber,voting, Datetime)
                
            elif (TDP==1):
                GPIO.output(15, False)
                print('else2')
                
            

    except Exception as e:
        print(e.message, e.args)
        print('Operation failed!')
        
        
        
while True:
    if not video_capture.isOpened():
        print('Unable to load camera.')
        sleep(2)
        pass

    # Capture frame-by-frame
    ret, frame = video_capture.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.2,
        minNeighbors=5,
        minSize=(30, 30)
    )

    # Draw a rectangle around the faces
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
        if i%100 == 0:
             cv2.imwrite("entry/"+name +'.'+ str(i) + ".jpg", frame)
             filename= name +'.'+ str(i) + ".jpg"
             # upload File to S3
             file_key_name = folder + '/' + filename
             local_path = os.getcwd()
             local_name = local_path + '/' + key_name + filename
             upload = s3_connect.upload_file(local_name, s3_bucket, file_key_name)
             compare()
             for key in q:
                 delete_faces(s3_bucket, key, region="us-east-1")
                 q.clear()
           
        i=i+1
        

    if anterior != len(faces):
        anterior = len(faces)
        log.info("faces: "+str(len(faces))+" at "+str(dt.datetime.now()))
        


    # Display the resulting frame
    cv2.imshow('Video', frame)


    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    # Display the resulting frame
    cv2.imshow('Video', frame)

# When everything is done, release the capture
video_capture.release()
cv2.destroyAllWindows()
