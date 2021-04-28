"""Provides a scripting component.
    Inputs:
        x: The x script variable
        y: The y script variable
    Output:
        a: The a output variable"""

__author__ = "jenniferjacobs"

import rhinoscriptsyntax as rs
from datetime import datetime
import math as math

def makeGcodeInfo(sb):
    sb.append("; " + datetime.now().strftime("%y%m%d_%H%M%S") + "\n")
    
def makeGcodeHeatSettings(sb):
    sb.append("M140 S" + str(BedTemp) +" ; Set Bed Temperature\n")
    sb.append("M105\n")
    sb.append("M190 S" + str(BedTemp) +" ; Wait for Bed Temperature\n")
    sb.append("M104 S" + str(NozzleTemp) +" ; Set Nozzle Temperature\n")
    sb.append("M105\n")
    sb.append("M109 S" + str(NozzleTemp) +" ; Wait for Nozzle Temperature\n")
    
def makeGcodeStartupSettings(sb):
    sb.append("G28\n")
    sb.append("M83\n")
    sb.append("G90\n")
    sb.append("G1 F300 Z0.4\n")
    sb.append("G1 X50 E8 F800\n")
    

def WriteFile(str, path):
    
    f = open(path, "w")
    f.write(str)
    f.close()

def makeRetraction(amount, speed, sign):
    mes = "";
    if(sign == -1):
        mes = " ; Retraction"
    else:
        mes = " ; Extraction"
    return "G1 F" + str(speed) + " E" + str(sign * amount) + mes + "\n"

def makeGcodePoints(fr, to):
    l = (to - fr).Length;
    numerator = (_NozzleWidth * l * _LayerHeight)
    denominator = (1.75 / 2) * (1.75 / 2) * Math.PI
    e = numerator / denominator
    return "G1 F1200 X" + "{:.{}f}".format(to.X,2) + " Y" + "{:.{}f}".format(to.Y,2) + " E" + "{:.{}f}".format(e,8) + "\n";

def makeGcodeSpeed(fr, to, speed):
    l = (to - fr).Length
    numerator = (_NozzleWidth * l * _LayerHeight)
    denominator = (1.75 / 2) * (1.75 / 2) * math.pi

    e = numerator / denominator

    return "G1 F" + str(speed) + " X" + "{:.{}f}".format(to.X,2) + " Y" + "{:.{}f}".format(to.Y,2) + " E" + "{:.{}f}".format(e,8) + "\n"

def makeGcode(to):
    return "G0 F9000 X" + "{:.{}f}".format(to.X,2) + " Y" + "{:.{}f}".format(to.Y,2) + "\n" 
    



_LayerHeight = LayerHeight;
_NozzleWidth = 0.4;
_RetAmount = 6.0;
_RetSpeed = 1200;
PrintSpeed = 500;
PrintSpeedHigh = 1000;  

if(Apply == True):
    sb = []
    makeGcodeInfo(sb)
    makeGcodeHeatSettings(sb)
    makeGcodeStartupSettings(sb)
    sb.append(makeRetraction(_RetAmount, _RetSpeed, -1))
   
    CurrentHeight = 0;
    i = 0

    #Pillar generation
    GAP = 3
    LayerNum = 0
    HairNum = 0
    i = 0

    for i in range(0, len(MeshPoints.Paths)):
        pList = MeshPoints.Branch(i)
        
        if(len(pList) != 0):
            CurrentHeight = pList[0].Z
            sb.append("G1 F300 Z" + str(CurrentHeight) + "\n")
            sb.append(makeGcode(pList[0]))
            sb.append(makeRetraction(_RetAmount, _RetSpeed, 1))

            j = 0
            for j in range(0,len(pList)):
                if(j == 0):
                    #sb.Append("G4 P100\n");
                    sb.append(makeGcode(pList[j]))
                    continue

                print(LayerHeight)
                if(pList[j].Z > CurrentHeight + (_LayerHeight / 10)):
                    CurrentHeight = pList[j].Z
                    sb.append("G1 F300 Z" + "{:.{}f}".format(CurrentHeight,3) + "\n")

                sb.append(makeGcodeSpeed(pList[j - 1], pList[j], PrintSpeed))

            sb.append(makeGcodeSpeed(pList[len(pList) - 1], pList[0], PrintSpeed))

        sb.append(makeRetraction(_RetAmount, _RetSpeed, -1))





    sb.append(makeRetraction(_RetAmount, _RetSpeed, -1));
    CurrentHeight += 10;
    sb.append("G1 Z" + "{:.{}f}".format(CurrentHeight,3) + "\n");


    sb.append("M84");
    strsb = ''.join(sb)
    WriteFile(strsb, FilePath);  
        
        
    a = strsb