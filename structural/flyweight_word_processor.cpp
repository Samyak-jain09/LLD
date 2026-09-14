#include <bits/stdc++.h>
using namespace std;

class IWord{
    public:
        virtual ~IWord() = default;
        virtual void display(int row, int column) = 0;
};

class DocumentCharacter : public IWord {
    private:
        int fontSize;
        string fontType;
        char character;
    public:
        DocumentCharacter(int sz, string type, char charc) : fontSize(sz), fontType(type), character(charc) {}
        void display (int row, int column){
            cout<<"Display character " << character << " at x and y as "<< row << " " << column << endl;
        }
};

class LetterFactory{
    private:
        inline static unordered_map<char, unique_ptr<IWord> > cache;
    public:
        static IWord* createLetter(char character){
            if(cache.find(character) == cache.end()){
                cache[character] = make_unique<DocumentCharacter> (10,"Arial",character);
            }
            return cache[character].get();
        }
};

int main(){
      /*
            this is the data we want to write into the word processor.

            Total = 58 characters
            t = 7 times
            h = 3 times
            a = 3 times and so on...

         */
    IWord* char1 = LetterFactory::createLetter('t');
    char1->display(0,0);
    IWord* char2 = LetterFactory::createLetter('t');
    char2->display(0,6);
    return 0;
}