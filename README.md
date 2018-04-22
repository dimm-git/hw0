OTUS C++ course  

The "kkmeans" project  

Usage:
kgen 0 > data.txt - to generate data like kkmeans_ex.cpp
kgen 1 > data.txt - to generate 'starry sky' data
  
cat kkmeans_ex.txt | kkmeans n - produce classified data output
  
Visualization:  
2-field data:
cat data.txt | kvis output.png
  
3-field (x,y,class) data:
cat data.txt | kkmeans n | kvis output.png
  
  
Debian package data:
/usr/share/kkmeans/stars.txt - 'starry sky' data
/usr/share/kkmeans/stars.png - 'starry sky' diagram
/usr/share/kkmeans/stars_class.txt - 'starry sky' classified data
/usr/share/kkmeans/stars_class.png - 'starry sky' classified diagram
/usr/share/kkmeans/kkmeans_ex.txt - 'example-like' data
/usr/share/kkmeans/kkmeans_ex.png - 'example-like' diagram
/usr/share/kkmeans/kkmeans_ex_class.png - 'example-like' classified data diagram
