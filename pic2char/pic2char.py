from PIL import Image
import argparse

parser=argparse.ArgumentParser()

parser.add_argument('file')
parser.add_argument('-o','--output')
parser.add_argument('--width',type=int,default=80)
parser.add_argument('--height',type=int,default=80)
parser.add_argument('--color',default='blue')

args=parser.parse_args()

IMG=args.file
WIDTH=args.width
HEIGHT=args.height
OUTPUT=args.output
COLOR=args.color

COLOR_DIC={'black':'30','red':'31','green':'32','yellow':'33','blue':'34','purple':'35'}

ascii_char = list("$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ")

def get_char(r,g,b,alpha=256):
	if alpha==0:
		return ' '
	length=len(ascii_char)
	gray=int(0.2126*r+0.7152*g+0.0722*b)
	unit=(256.0+1)/length;
	return ascii_char[int(gray/unit)]

if __name__=='__main__':
	im=Image.open(IMG)
	print im.size
	im=im.resize((WIDTH,HEIGHT),Image.NEAREST)

	txt=""

	for i in range(HEIGHT):
		for j in range(WIDTH):
			txt+=get_char(*im.getpixel((j,i)))
		txt+='\n'

	print '\033['+COLOR_DIC[COLOR]+'m'+txt+'\033[0m'

	if(OUTPUT):
		fd=open(OUTPUT,'w')
		fd.write(txt)