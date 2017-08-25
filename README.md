np-cvthings
==============
This is a collection of objects, sketches and studies that i use as foundation for further creative coding works with openFrameworks. Mainly things based on kinect or opencv. Not all the classes have a clean API and i will change the interface as many time i need, until i eventually i make separate addons from the most useful and stable parts. Otherwise differently stated he code is licensed under GNU GLPv3 (some apps are MIT-licensed, there is written in the ofApp.cpp code). 
   
The repo folders goes into the oF addons folder. Everything is tested only on Linux, although many classes should work on other operative systems too... if it works it works, otherwise you have to figure it yourself, no support will be given.
   
for making openCv work with the file load dialog used your have to remove the gtk3 dev headers, otherwise you will get exception in runtime crashing the program    
```
sudo apt-get remove libgtk-3-dev
sudo apt-get install libgtk2.0-dev
```  
      
for ofxCv to work on oF stable, remember to install glm-dev  
`sudo apt-get install libglm-dev`
and checkout the stable branch  
```
cd ~/oF/addons/ofxCv
git checkout stable
```

