# tarea3.mk

tarea3.pdf : tarea3.tex tiempo=60s.png tiempo=30s.png 
	pdflatex tarea3.tex

tiempo=60s.png : Onda.py
	python Onda.py

tiempo=30s.png : Onda.py
	python Onda.py
