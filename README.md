# classifyhandshape
A C++ implementation of hand shape recognition by template matching

Problem Definition:
The goal of current programming assignment is to develop a classifier that could identify the hand shape captured by a web cam in real-time by template matching method.

Method and Implementation:
A template matching program was developed to indentify 5 hand shapes: open palm, fist, thumb up, okay sign, and gun sign. Firstly, 100 templates of these hand shape were collected from web cam. Secondly, confusion matrix evaluation was performed from web cam videos of known hand shape. Thirdly, a demo program was implemented which enables real-time tagging of the hand shape from webcam.

1) Template collection ("train.cpp") An interactive program was developed to collect user's hand image, extract the skin part from the image background, and compile the results in a template collection ("template.png"). In the beginning, the program displays a black screen and text of the hand shape required. When the user is ready, he/she presses a button and after 3 seconds, the program starts to collect a total of 100 images from the web cam as the templates for the specific hand shape. To increase the robustness ,the user needs to rotate his/her hand, and change the relative position of hand to the camera, which helps the algorithm learn difference appearance of the hand shape. After the program repeats the template collection for each hand shape, the templates are down sampled to smaller iamge (width/height: 160 pixels). The down-sampled images are compiled into a single large image matrix file, which has 20 rows, 25 columns of hand shape templates.

2) Evaluation ("testResult.cpp") An interactive program was developed to collect user's hand image and test the accuracy of template matching. Similar to the setup of template collection, the program asks the user to perform certain hand shape, and the image frames from web cam were binarize to extract the skin part. The binary skin image were down-sampled to a smaller image (width/height: 160 pixels), and matched to the template image by sum of square difference method. The location of the best matched image indicates the type of hand shapes. The classification results were evaluated in the confusion matrix.

3) Demo ("demo.cpp") A demo program was developed to show the real time tagging of hand shape. The program utilizes the skin detection and classification to provide the user a real-time shape recognition of his/her hand.

Experiments (Mac)
Prerequisite: CMake and OpenCV

Demo:
1) Open Terminal and navigate to the folder
2) type "mkdir build" and return
3) type "cd build" and return
4) copy the template.png into the buld foder
5) type "./demo" and return

Get your own hand template:
1) Repeat the steps 1-5 above
2) type "./train" and return
3) follow the instruction to do the 5 hand shapes (fist, palm, thumb up, ok sign, gun sign) in the rectangle area. 
4) Change the appearance( position, distance to camera) of hand, so templates of different appearance will be collected.
5) After the recording is complete, you will find a template.png in the folder, which is a image matrix of all the templates.


