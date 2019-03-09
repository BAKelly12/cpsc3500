#include "FileSys.h"
#include <iostream>
using namespace std;


int main()
{
	FileSys fs;
	
	fs.mount(1);

	fs.create("file1");
	
	fs.append("file1","ADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADADAD");
	fs.stat("file1");
	fs.append("file1", "bcghy");
	fs.stat("file1");
	cin.get();
	
	
	/*
	fs.ls();
	cin.get();
	fs.mkdir("dir1");	
	cin.get();
	fs.mkdir("dir2");
	cin.get();
	fs.create("file9");
	cin.get();
	fs.ls();
	cin.get();
	
	fs.cd("dir1");
	cin.get();
	fs.create("file1");
	cin.get();
	
	fs.append("file1","helloworld!");
	cin.get();
	fs.stat("file1");
	cin.get();

	fs.ls();
	cin.get();
	fs.cat("file1");
	cin.get();
	fs.head("file1",5);
	cin.get();
	
	fs.rm("file2");
	cin.get();
	fs.cat("file2");
	cin.get();

	fs.create("file1");
	cin.get();
	fs.create("file2");
	cin.get();
	fs.create("file3");	
	
	fs.ls();
	cin.get();

	fs.rm("file1");
	cin.get();

	fs.ls();
	cin.get();
	fs.home();
	cin.get();
	fs.ls();
	cin.get();

	fs.stat("dir1");
	cin.get();

	fs.rmdir("dir3");
	cin.get();
	fs.rmdir("dir1");
	cin.get();
	fs.rmdir("dir2");
	cin.get();
	fs.ls();
	cin.get();
	*/
	
	fs.unmount();
}
