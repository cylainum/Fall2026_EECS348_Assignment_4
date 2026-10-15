.PHONY: clean program

program: checker

checker: ChatGPT Gemini

ChatGPT: ChatGPT.o
	g++ ChatGPT.o -o ChatGPT

Gemini: Gemini.o
	g++ Gemini.o -o Gemini

ChatGPT.o: ChatGPT.cpp
	g++ -c ChatGPT.cpp

Gemini.o: Gemini.cpp
	g++ -c Gemini.cpp

clean:
	rm -f ChatGPT.o Gemini.o Gemini ChatGPT
