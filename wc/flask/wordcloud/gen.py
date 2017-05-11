from os import path
from scipy.misc import imread
from wordcloud import WordCloud, STOPWORDS, ImageColorGenerator
import jieba

def cutword(text):
	segment = jieba.cut(text)
	segment = [item for item in segment if item != u' ']
	segment = r' '.join(segment)
	return segment

text = open('text', encoding='utf-8').read()

text = cutword(text)

back_mask = imread('image/love.jpg')
alice_coloring = imread('image/alice.png')
image_colors = ImageColorGenerator(alice_coloring)

wc = WordCloud(	font_path='font/YaHeiConsolas.ttf',
				background_color='white',
				max_words=2000,
				mask=back_mask,
				random_state=42
				)

wc.generate(text)

wc.to_file("result.png")