# Test harness for rectangleOverlap
# To progress to the next test case, type ESC

# Compile C++ example like this
#   g++ -std=c++17 -o rectangleOverlap  rectangleOverlap.cpp main.cpp

# Then change the following Python line to make path point to your own executable
path = '/Users/matt/src/viewportOverlap/rectangleOverlap/DerivedData/rectangleOverlap/Build/Products/Debug/rectangleOverlap'

# Test cases:
# Each test case is a list of the form 
#              [x1,y1,w1,h1,r1, x2,y2,w2,h2,r2]
testVectors = [[2, 4.5, 2, 2, 0, 2, 6, 4, 2, 45],
               [1, 1, 1, 1, 0, 5, 5, 1, 1, 0],
               [4, 4, 4, 4, 0, 7, 4, 4, 4, 0], 
               [4, 6, 4, 2, 45, 6, 4, 2, 2, 0],
               [4, 6, 4, 2, 20, 6, 4, 4, 2, 20],
               [4, 6, 4, 2, -70, 6, 4, 4, 2, -70],
               [4, 7, 4, 2, -70, 6, 4, 4, 2, -70],
               [4, 8, 4, 2, -70, 6, 4, 4, 2, -70], 
               [5, 7, 4, 2, 0, 5, 4, 2, 2, 45],
               [4, 6, 4, 2, 45, 6, 5, 2, 2, 0],
               [4, 6, 4, 2, 45, 6, 5, 2, 2, 45],
               [4, 6, 4, 2, 45, 6, 5, 2, 2, 35], 
               [5, 5, 2, 2, 0, 5, 5, 4, 4, 0],
               [5, 5, 4, 4, 0, 5, 5, 2, 2, 0],
               [5, 'y', 4, 4, 0, 5, 5, 2, 2, 0],
               [4, 6, 4, 2, 30, 6, 5, 2, 2, 60],
               [6, 4, 2, 2, 0, 4, 6, 4, 2, 45],
               [6, 4, 2, 2, 0, 2, 6, 4, 2, 45],
               [6, 4, 2, 2, 0, ' ', 6, 4, 2, 45],
               [2, 0.6, 2, 2, 0, 2, 6, 4, 2, 45]
               ]
# where
#      (x1,y1) = center of rect1
#      (w1,h1) = width and height of rect1
#      r1      = rotation of rect1 in degrees
#      (x2,y2) = center of rect2
#      (w2,h2) = width and height of rect2
#      r2      = rotation of rect2 in degrees

import subprocess
import matplotlib.pyplot as plt
import re

# Regex patterns used to parse output of executable under test
r1pat = re.compile('\nRect1:\nRectangle vertices: \(([\d\.\+\-]+),\s+([\d\.\+\-]+)\)\s+\(([\d\.\+\-]+),\s+([\d\.\+\-]+)\)\s+\(([\d\.\+\-]+),'
                   + '\s+([\d\.\+\-]+)\)\s+\(([\d\.\+\-]+),\s+([\d\.\+\-]+)\)')
r2pat = re.compile('\nRect2:\nRectangle vertices: \(([\d\.\+\-]+),\s+([\d\.\+\-]+)\)\s+\(([\d\.\+\-]+),\s+([\d\.\+\-]+)\)\s+\(([\d\.\+\-]+),'
                   + '\s+([\d\.\+\-]+)\)\s+\(([\d\.\+\-]+),\s+([\d\.\+\-]+)\)')
yesOverlap = re.compile('Rectangles are overlapped\n')
noOverlap = re.compile('(Rectangles separated by axis [\d\.\+\- \(\),]+?) +\n')
error = re.compile('Usage')

def plotRect(rect):
    # Plot a rectangle
    temp = rect.copy()
    temp.append(temp[0])
    xs, ys = zip(*temp)
    plt.plot(xs, ys)
    
def press(event):
    if event.key == "escape":
        plt.close()
    
def runTest(x1, y1, w1, h1, r1, x2, y2, w2, h2, r2):
    # Convert input parameters to strings
    x1 = str(x1)
    y1 = str(y1)
    w1 = str(w1)
    h1 = str(h1)
    r1 = str(r1)
    
    x2 = str(x2)
    y2 = str(y2)
    w2 = str(w2)
    h2 = str(h2)
    r2 = str(r2)
    
    # Create axes for plotting
    fig, ax = plt.subplots()
    fig.canvas.mpl_connect('key_press_event', press)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_xlim(0, 10)
    ax.set_ylim(0, 10)
    ax.set_aspect('equal')             
    
    # run the program under test and get result from stdout
    result = subprocess.run([path, x1, y1, w1, h1, r1, x2, y2, w2, h2, r2], capture_output=True, text=True)
    r = result.stdout
    
    match = re.search(error, r)     # check for errors reported by executable under test
    if (match is not None):
        ax.set_title('Bad input parameters')
        plt.show()
    else:
        # parse the result to extract coordinates of rect1 and rect2 as well as their overlap status
        match = re.search(r1pat, r)
        x1 = float(match.group(1))
        y1 = float(match.group(2))
        x2 = float(match.group(3))
        y2 = float(match.group(4))
        x3 = float(match.group(5))
        y3 = float(match.group(6))
        x4 = float(match.group(7))
        y4 = float(match.group(8))
        rect1 =  [[x1, y1], [x2, y2], [x3, y3], [x4, y4]]
        plotRect(rect1)
        
        match = re.search(r2pat, r)
        x1 = float(match.group(1))
        y1 = float(match.group(2))
        x2 = float(match.group(3))
        y2 = float(match.group(4))
        x3 = float(match.group(5))
        y3 = float(match.group(6))
        x4 = float(match.group(7))
        y4 = float(match.group(8))
        rect2 =  [[x1, y1], [x2, y2], [x3, y3], [x4, y4]]
        plotRect(rect2)
        
        # Check for overlap
        yo = re.search(yesOverlap, r)
        no = re.search(noOverlap, r) 
    
        if yo is not None:
            ax.set_title('It overlaps')
        elif no is not None:
            ax.set_title(no.group(1))
        else:
            ax.set_title('Error: This should never happen')
        
        plt.show()

for tv in testVectors:
    x1, y1, w1, h1, r1, x2, y2, w2, h2, r2 = tv
    runTest(x1, y1, w1, h1, r1, x2, y2, w2, h2, r2)