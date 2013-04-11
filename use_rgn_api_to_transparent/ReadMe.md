
  
##透明窗口的另一种实现方式
一年半以前（2011.10），接触到了BitmapToRegion函数，该函数可以把位图指定RGB值的点透明化。所以使用这个函数可以实现透明窗口效果，这是SetLayeredWindowAttributes函数之外的第二种实现方式，不过BitmapToRegion的主要用途是创造异形窗口，所以无法实现部分透明效果。  
BitmapToRegion的优点是：不会出现闪烁。如果使用SetLayeredWindowAttributes实现，当背景窗口不断变化刷新时（譬如当背景窗口是播放器时）则会出现闪烁的问题。


BitmapToRegion函数是由Jean-Edouard Lachand-Robert在1998年实现的，该函数有些小瑕疵，我对它做了小修改，详细参见代码。  
cswuyg@gmail.com