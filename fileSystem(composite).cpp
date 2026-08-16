#include <bits/stdc++.h>
using namespace std;

class FileSystem{
    public:
        virtual void ls() = 0;
        virtual ~FileSystem() = default;
};

class File : public FileSystem{
    public:
        File(string name) : name(name) {}
        void ls() override {
            cout << "File: " << name << endl;
        }
    private:
        string name;
};

class Directory : public FileSystem{
    public:
        Directory(string name) : name(name) {}
        void add (FileSystem* fs) {
            children.push_back(fs);
        }
        void ls() override{
            cout<<"Directory: " << name << endl;
            for(auto child: children){
                child->ls();
            }
        }
    private:
        string name;
        vector<FileSystem*> children;
};

int main(){
    Directory* root = new Directory("root");
    File* file1 = new File("file1.txt");
    File* file2 = new File("file2.txt");
    Directory* subDir = new Directory("subdir");
    File* file3 = new File("file3.txt");
    root->add(file1);
    root->add(file2);
    root->add(subDir);
    subDir->add(file3);
    root->ls();
    delete root;
    return 0;
}