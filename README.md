# Graphical-editor-for-EasyMX-Pro-v.7-board
Windows Paint like program written in C. <br>
Display is divided into 2 parts - the writable part where user can write whatever he wants and part containing features like colors or fonts (user can't write anything here).

Note: To run this program you need to install EasyMX Pro v.7 Emulator in order for this program to work correctly. To run the program use editor.c file in any C/C++ editor. Others contains libraries and drivers needed to emulate EasyMX Pro v.7 board' CPU behaviour and other elements that are later included in "executable" code (editor.c).

Code desctiption: <br>
General Purpose Variables: <br><br>
colorPos - 2D table that contains colors coordinates on display. It is closely linked to "color "table <br><br>
fontPos - 2D table that contains fonts coordinates on display <br><br>
shapePos - 2D table that contains shapes coordinates on display <br><br>
color - 1D table containing unsigned long variables representing different colors.

General Purpose Functions: <br><br>
drawRect() - displays a rectangle in given coordinates, color and font width. <br>
drawTriangle() - displays a triangle which top apex has coordinates given as arguments filled with given color and font width. <br><br>
drawCircle() - displays a circle which center is at given coordinates filled with given color and font width. <br><br>
RectInRect() - this is an auxilary function that makes overall code more transparent. It fills given rectangle with other smaller rectangles. It is used to display font icons. <br><br>
drawLineIcon() - auxilary function that divides the display into 2 parts. <br><br>
initScreen() - writes on screen all icons that represents program's functionality. <br><br>
ifColor() - checks if user at the moment points at color icons. <br><br>
ifFont() - checks if user at the moment points at font icons. <br><br>
ifShape() - checks if user at the moment points at shape icons. <br><br>
drawShape() - draws chosen shape on screen at given point. <br><br>
TouchCallback() - the most important function. It enables to use touchpad and track pointer location. It also distinguishes whether the user tapped the screen first time (ulMessage=2), holds the pointer (ulMessage=3) or released the pointer (ulMessage=4). <br><br>
main() - contains "initScreen", touchpad, display, memory and CPU clock initialisation functions as well as infinite loop that prevents program from shutting down immediately. <br>
