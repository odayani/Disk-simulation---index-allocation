Disk simulation - index allocation method .
Name: Or Dayani
ID: 315710715

--Description--
This program simulate how disk manage the data by index allocation method.


--Functions--
void decToBinary(int n, char &c) ---> save the block number that we found for saving data file at char on inedx_block .
FsFile(int _block_size)  --> initializing the FsFile object for first time
int getfile_size() const --> get the size of the file 
int get_block_in_use() const --> get how many blocks in use for each file.
int get_index_block() const --> get the number of the index block that save the information about where the data exist .
void append_file_size(const int _file_size) --> add the size that the used added into the file on the file_size attribute of the file object.
void append_block_in_use(const int _block_in_use) --> add the number of blocks that the user add to the file on the block_in_use attribute of the file.
void set_index_block (int setter) --> set the index block when we want to enter data for the first time to the file .
bool hasBeenWritten() --> return if the file have information or the file is empty.

string getFileName() --> get the name of the file descriptor .
bool getInUse() const --> getting information about the use of the file .
void setUse(bool temp) --> changes the information afor using the file .
FsFile* get_Fsfile() --> get the fsfile.
void remove_from_openfd(int fd) --> remove the fileDescriptor from OpenfileDescriptors array.
int add(FileDescriptor *fileDescriptor) --> get fileDescriptor and add it to fileDescriptors array.
int delete_fd(FileDescriptor *fileDescriptor) --> delete fileDescriptor from fileDescriptors array.
FileDescriptor *get(int fd) --> return the fd number file descriptor from the file descriptors array.
get_by_name(string &filename) --> return the number of file descriptor from file descriptor array by his name.
int get_by_fd(FileDescriptor *fileDescriptor) --> get file descriptor and return his spot in thr file descriptors array.

int getIndex() const --> return the block index.
void setIndex(int _index) --> set the block index.
 char *getContent() const --> get the block information.
void setContent(char *_content) --> set the information the user add into the block .

int get_unoccupied_index_block() --> find first block that not in use .
void listAll() --> print all the data from the disk .
void fsFormat(int blockSize)  --> initializing the disk for the first time considering the block size .
int CreateFile(const string &fileName) --> create new file on the disk .
int OpenFile(const string &fileName) --> open the file on open file descriptors for add or read data from .
string CloseFile(int fd) --> close the file from open file descriptors .
int WriteToFile(int fd, char *buf, int len)  --> write data for the file when all the data goes into the disk.
int DelFile(const string &FileName) --> delete the file and all its information from the disk .
int ReadFromFile(int fd, char *buf, int len) --> read the information of the file from the disk .
 int write(FileDescriptor *fd, char *buf, int len) --> write the information that stay in the block , into the disk.
int get_index_of_index_block(FileDescriptor *fd) --> get the block index - block that save the indexes of the file data in the disk.
Block *getIndexBlock(FileDescriptor *fd) --> get index block for the file or make new block for first data .
Block *getAvailableBlock(FileDescriptor *fd) --> find empty block to store data into .
int write_block(Block *block) --> write the data that stay in the block into the disk.
int del_block(Block *block --> delete all the data from the block .
Block *read_block(int position) --> read all the data from the block .
void set_block_occupied(int index_of_data_block) --> set information of the block that we take in the bitVector.


--Program Files--
ex7_final_proj.2021.cpp - contain all the program 

--How to compile?--
compile:
g++ ex7.cpp -o v1
run: ./v1

--Input--
The user can delete or add information from the disk according to the options available in the menu .
first, user have to format the disk with the input "2" .

for example : 
input :
2 
4
3 
'FIRST_FILE_NAME'

explain :
2 will format the disk .
4 will open the file on the disk.
3 will create new file on the disk .
this will open a new file with name 'FIRST_FILE_NAME'

--Output--
index: 0: FileName: A , isInUse: 1
