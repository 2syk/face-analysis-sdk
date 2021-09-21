import build.lib.face_analysis_sdk as fa
import cv2

img = cv2.imread("testface.jpeg")
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

ft = fa.FaceTracker()
ft.Load("src/tracker/resources/face.mytracker.binary", True)

ftp = fa.LoadFaceTrackerParams("src/tracker/resources/face.mytrackerparams.binary")
result = ft.NewFrame(img, ftp)

pose = ft.getPose()
print("Pose: %s %s %s" % (pose.pitch, pose.yaw, pose.roll))

print ("2D Shape Points: %s" % ft.getShape())
print ("3D Shape Points: %s" % ft.get3DShape())