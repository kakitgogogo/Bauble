# -*- coding: utf-8 -*-
from graphics import *
from random import randint

MAX_NUM=1<<30

class gobang:
    def __init__(self):
        self.win=GraphWin('gobang',800,640)
        self.win.setBackground('white')

        self.points=[[0 for a in range(15)] for b in range(15)]
        for i in range(15):
            for j in range(15):
                self.points[j][i]=Point(40*(i+1),40*(j+1))

        self.board=[[0 for a in range(15)] for b in range(15)]

        self.direction=[[-1,0],[-1,1],[0,1],[1,1],[1,0],[1,-1],[0,-1],[-1,-1]]

        self.scores=[1,10,100,1000,10000,100000]

        self.Rec_goback=None
        self.Rec_surrender=None
        self.Rec_exit=None

        self.Rec_win=None
        self.Text_win=None

        self.Chess_Set=set()

    def welcome(self):
        Rec_PvP=Rectangle(Point(250,300),Point(550,350))
        Rec_PvP.setFill('white')
        Rec_PvP.draw(self.win)
        Text_PvP=Text(Point(400,325),'人人对战')
        Text_PvP.draw(self.win)

        Rec_PvC=Rectangle(Point(250,380),Point(550,430))
        Rec_PvC.setFill('white')
        Rec_PvC.draw(self.win)
        Text_PvC=Text(Point(400,405),'人机对战')
        Text_PvC.draw(self.win)

        Rec_Exit=Rectangle(Point(250,460),Point(550,510))
        Rec_Exit.setFill('white')
        Rec_Exit.draw(self.win)
        Text_Exit=Text(Point(400,485),'退出游戏')
        Text_Exit.draw(self.win)
        while True:
            ClickPoint=self.win.getMouse()
            x=ClickPoint.getX()
            y=ClickPoint.getY()

            if 250<=x<=550 and 300<=y<=350:
                Rec_PvP.undraw()
                Rec_PvC.undraw()
                Rec_Exit.undraw()
                Text_PvP.undraw()
                Text_PvC.undraw()
                Text_Exit.undraw()
                return 1
            elif 250<=x<=550 and 380<=y<=430:
                Rec_PvP.undraw()
                Rec_PvC.undraw()
                Rec_Exit.undraw()
                Text_PvP.undraw()
                Text_PvC.undraw()
                Text_Exit.undraw()
                return 2
            elif 250<=x<=550 and 460<=y<=510:
                self.win.close()
                return 3

    def background(self):
        for i in range(15):
            Line(self.points[i][0],self.points[i][-1]).draw(self.win)
        for i in range(15):
            Line(self.points[0][i],self.points[-1][i]).draw(self.win)
        
        CenterPoint=Circle(self.points[7][7],3)
        CenterPoint.draw(self.win)
        CenterPoint.setFill('black')

        self.Rec_goback=Rectangle(Point(640,80),Point(760,120))
        self.Rec_goback.setFill('white')
        self.Rec_goback.draw(self.win)
        Text(Point(700,100),'悔棋').draw(self.win)

        self.Rec_surrender=Rectangle(Point(640,160),Point(760,200))
        self.Rec_surrender.setFill('white')
        self.Rec_surrender.draw(self.win)
        Text(Point(700,180),'认输').draw(self.win)

        self.Rec_exit=Rectangle(Point(640,240),Point(760,280))
        self.Rec_exit.setFill('white')
        self.Rec_exit.draw(self.win)
        Text(Point(700,260),'退出').draw(self.win)

    def WinMessage(self,winner):
        self.Rec_win=Rectangle(self.points[4][4],self.points[10][10])
        self.Rec_win.setFill('white')
        self.Rec_win.draw(self.win)
        self.Text_win=Text(self.points[7][7],'The %s is the winner'%winner)
        self.Text_win.draw(self.win)

    def getNearest(self,x,y):
        xx=(x/40)*40
        yy=(y/40)*40
        if x-xx<20 and y-yy<20:
            x=xx
            y=yy
        elif x-xx>20 and y-yy<20:
            x=xx+40
            y=yy
        elif x-xx<20 and y-yy>20:
            x=xx
            y=yy+40
        else:
            x=xx+40
            y=yy+40
        return x/40-1,y/40-1

    def click(self,black_chess,white_chess,x1,y1,x2,y2):
        while True:
            ClickPoint=self.win.getMouse()
            x=ClickPoint.getX()
            y=ClickPoint.getY()

            if 640<=x<=760 and 80<=y<=120 and x1>=0 and y1>=0:
                black_chess.undraw()
                white_chess.undraw() 
                self.board[x1][y1]=0
                self.board[x2][y2]=0
                return -1,-1,None

            elif 640<=x<=760 and 160<=y<=200:
                return -2,-2,None

            elif 640<=x<=760 and 240<=y<=280:
                return -3,-3,None

            x,y=self.getNearest(x,y)
            if y<0 or x<0 or y>14 or x>14:continue
            if self.board[y][x]==0:
                self.board[y][x]=1
                chess=Circle(self.points[y][x],18)
                self.Chess_Set.add(chess)
                chess.draw(self.win)
                chess.setFill('black')
                chess.setOutline('black')
                return x,y,chess

    def PvP_Click(self,color):
        while True:
            ClickPoint=self.win.getMouse()
            x=ClickPoint.getX()
            y=ClickPoint.getY()

            if 640<=x<=760 and 160<=y<=200:
                return -2,-2

            elif 640<=x<=760 and 240<=y<=280:
                return -3,-3

            ColorNum=1
            if color=='white':
                ColorNum=2

            x,y=self.getNearest(x,y)
            if y<0 or x<0 or y>14 or x>14:continue
            if self.board[y][x]==0:
                self.board[y][x]=ColorNum
                chess=Circle(self.points[y][x],18)
                self.Chess_Set.add(chess)
                chess.draw(self.win)
                chess.setFill(color)
                chess.setOutline('black')
                return x,y

    def check(self,x,y,ChessColor):
        if ChessColor=='w':
            color=2
        else:
            color=1
 
        for i in range(4):
            count=0
            yy,xx=y,x
            while self.board[xx][yy]==color:
                    xx=xx+self.direction[i][0]
                    yy=yy+self.direction[i][1]
                    count+=1
                    if yy<0 or xx<0 or yy>14 or xx>14:break
            yy,xx=y,x
            while self.board[xx][yy]==color:
                    xx=xx+self.direction[i+4][0]
                    yy=yy+self.direction[i+4][1]
                    count+=1
                    if yy<0 or xx<0 or yy>14 or xx>14:break
            if count-1>=5:return True
        return False

    def computer_click(self,x,y):
        #print x,y
        self.board[x][y]=2
        chess=Circle(self.points[x][y],18)
        self.Chess_Set.add(chess)
        chess.draw(self.win)
        chess.setFill('white')
        chess.setOutline('black')
        return chess


    def score(self,CurBoard,ChessColor):
        if ChessColor=='w':
            color=2
            other=1
        else:
            color=1
            other=2
        ScoreTable=[[0 for a in range(15)] for b in range(15)]
        sum_board=0
        Max,X,Y=0,0,0
        for x in range(15):
            for y in range(15):
                if CurBoard[x][y]!=0:
                    ScoreTable[x][y]=-1
                    continue
                for i in range(4):
                    count=0
                    other_count=0
                    bound_count=0
                    space_count=0
                    more_count1,more_count2=0,0
                    more_bound1,more_bound2=0,0
                    yy,xx=y,x
                    while True:
                        xx=xx+self.direction[i][0]
                        yy=yy+self.direction[i][1]
                        count+=1
                        if yy<0 or xx<0 or yy>14 or xx>14:
                            bound_count+=1
                            break
                        if CurBoard[xx][yy]==other:
                            other_count+=1
                            break
                        if CurBoard[xx][yy]==0:
                            space_count=1
                            break
                    if space_count==1:
                        xx=xx+self.direction[i][0]
                        yy=yy+self.direction[i][1]
                        while 0<=xx<=14 and 0<=yy<=14 and CurBoard[xx][yy]==color:
                            more_count1+=1
                            xx=xx+self.direction[i][0]
                            yy=yy+self.direction[i][1]
                            if 0<=xx<=14 and 0<=yy<=14 and CurBoard[xx][yy]==0:
                                space_count+=1
                                break
                    if space_count==1:more_bound1=1

                    space_count=0
                    yy,xx=y,x
                    while True:
                        xx=xx+self.direction[i+4][0]
                        yy=yy+self.direction[i+4][1]
                        count+=1
                        if yy<0 or xx<0 or yy>14 or xx>14:
                            bound_count+=1
                            break
                        if CurBoard[xx][yy]==other:
                            other_count+=1
                            break
                        if CurBoard[xx][yy]==0:
                            space_count=1
                            break
                    if space_count==1:
                        xx=xx+self.direction[i+4][0]
                        yy=yy+self.direction[i+4][1]
                        while 0<=xx<=14 and 0<=yy<=14 and CurBoard[xx][yy]==color:
                            more_count2+=1
                            xx=xx+self.direction[i+4][0]
                            yy=yy+self.direction[i+4][1]
                            if 0<=xx<=14 and 0<=yy<=14 and CurBoard[xx][yy]==0:
                                space_count+=1
                                break
                    if space_count==1:more_bound2=1

                    more_count=max(more_count1-more_bound1,more_count2-more_bound2)

                    count-=1
                    other_count+=bound_count
                    if other_count==2:
                        ScoreTable[x][y]+=0
                    elif more_count>0:
                        count+=more_count
                        if count==3:
                            ScoreTable[x][y]+=self.scores[3]
                            #print ChessColor,":spacial score(%d %d):"%(x,y),ScoreTable[x][y]
                        else:
                            ScoreTable[x][y]+=0
                    elif count>=5: 
                        ScoreTable[x][y]+=self.scores[5]
                    elif count==4 and other_count==0:
                        ScoreTable[x][y]+=self.scores[4]
                    elif count==4 and other_count==1:
                        ScoreTable[x][y]+=self.scores[3]+1
                    elif count==3 and other_count==0:
                        ScoreTable[x][y]+=self.scores[3]+1
                    elif count==3 and other_count==1:
                        ScoreTable[x][y]+=self.scores[2]
                    elif count==2 and other_count==0:
                        ScoreTable[x][y]+=self.scores[2]
                    elif count==2 and other_count==1:
                        ScoreTable[x][y]+=self.scores[1]
                    elif count==1 and other_count-bound_count==1:
                        ScoreTable[x][y]+=self.scores[0]
                    else:
                        ScoreTable[x][y]+=0
                sum_board+=ScoreTable[x][y]
                if ScoreTable[x][y]>Max:
                    X,Y=x,y
                    Max=ScoreTable[x][y]
        return sum_board,Max,X,Y

    def computer(self):
        sum_white,Max1,x1,y1=self.score(self.board,'w')
        sum_black,Max2,x2,y2=self.score(self.board,'b')
        #print "黑子得分：",sum_black
        #print "白子得分：",sum_white
        if Max1>=self.scores[5]:
            return x1,y1
        elif Max2>=self.scores[5]:
            return x2,y2
        elif Max1>=self.scores[4]:
            return x1,y1
        elif Max2>=self.scores[4]:
            return x2,y2
        elif Max1>=2*self.scores[3]:
            return x1,y1
        elif Max2>=2*self.scores[3]:
            return x2,y2
        elif Max1>=self.scores[3]:
            return x1,y1
        elif Max2>=self.scores[3]:
            return x2,y2
        elif Max1>=self.scores[2]:
            return x1,y1
        elif Max2>=self.scores[2]:
            return x2,y2
        elif Max1>=self.scores[1]:
            return x1,y1
        elif Max2>=self.scores[1]:
            return x2,y2
        else:
            return x1,y1

    def restart(self,func):
        self.win.setBackground('white')
        self.Rec_win.undraw()
        self.Text_win.undraw()

        for chess in self.Chess_Set:
            chess.undraw()

        self.board=[[0 for a in range(15)] for b in range(15)]

        func()

    def PvP(self):
        self.background()
        x1=y1=x2=y2=-1
        while True:
            x1,y1=self.PvP_Click('black')
            if x1==-2:
                self.WinMessage('white')
                break
            elif x1==-3:
                break
            if self.check(y1,x1,'b'):
                self.WinMessage('black')
                break
            x2,y2=self.PvP_Click('white')
            if x2==-2:
                self.WinMessage('black')
                break
            elif x2==-3:
                break
            if self.check(y2,x2,'w'):
                self.WinMessage('white')
                break
        if x1>=-2:
            self.win.getMouse()
            self.restart(self.PvP)
        else:
            self.win.close()

    def PvC(self):
        self.background()
        black_chess=None
        white_chess=None
        x1=y1=x2=y2=-1
        while True:
            x1,y1,black_chess=self.click(black_chess,white_chess,y1,x1,x2,y2)
            if x1==-1:continue
            elif x1==-2:
                self.WinMessage('white')
                break
            elif x1==-3:
                break
            if self.check(y1,x1,'b'):
                self.WinMessage('black')
                break
            x2,y2=self.computer()
            white_chess=self.computer_click(x2,y2)
            if self.check(x2,y2,'w'):
                self.WinMessage('white')
                break
        if x1>=-2:
            self.win.getMouse()
            self.restart(self.PvC)
        else:
            self.win.close()

    def run(self):
        choice=self.welcome()
        if choice==1:
            self.PvP()
        elif choice==2:
            self.PvC()
        else:
            return

def main():
    game=gobang()
    game.run()

if __name__=="__main__":
    main()