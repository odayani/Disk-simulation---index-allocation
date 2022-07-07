Disk simulation - index allocation method .

--Description--
This program simulate how disk manage the data by index allocation method.

--Functions--
:heavy_check_mark: void decToBinary(int n, char &c) ---> save the block number that we found for saving data file at char on inedx_block .
:heavy_check_mark: FsFile(int _block_size)  --> initializing the FsFile object for first time
:heavy_check_mark: int getfile_size() const --> get the size of the file 
:heavy_check_mark: int get_block_in_use() const --> get how many blocks in use for each file.
:heavy_check_mark: int get_index_block() const --> get the number of the index block that save the information about where the data exist .
:heavy_check_mark: void append_file_size(const int _file_size) --> add the size that the used added into the file on the file_size attribute of the file object.
:heavy_check_mark: void append_block_in_use(const int _block_in_use) --> add the number of blocks that the user add to the file on the block_in_use attribute of the file.
:heavy_check_mark: void set_index_block (int setter) --> set the index block when we want to enter data for the first time to the file .
:heavy_check_mark: bool hasBeenWritten() --> return if the file have information or the file is empty.

:heavy_check_mark: string getFileName() --> get the name of the file descriptor .
:heavy_check_mark: bool getInUse() const --> getting information about the use of the file .
:heavy_check_mark: void setUse(bool temp) --> changes the information afor using the file .
:heavy_check_mark: FsFile* get_Fsfile() --> get the fsfile.
:heavy_check_mark: void remove_from_openfd(int fd) --> remove the fileDescriptor from OpenfileDescriptors array.
:heavy_check_mark: int add(FileDescriptor *fileDescriptor) --> get fileDescriptor and add it to fileDescriptors array.
:heavy_check_mark: int delete_fd(FileDescriptor *fileDescriptor) --> delete fileDescriptor from fileDescriptors array.
:heavy_check_mark: FileDescriptor *get(int fd) --> return the fd number file descriptor from the file descriptors array.
:heavy_check_mark: get_by_name(string &filename) --> return the number of file descriptor from file descriptor array by his name.
:heavy_check_mark: int get_by_fd(FileDescriptor *fileDescriptor) --> get file descriptor and return his spot in thr file descriptors array.

:heavy_check_mark: int getIndex() const --> return the block index.
:heavy_check_mark: void setIndex(int _index) --> set the block index.
:heavy_check_mark: char *getContent() const --> get the block information.
:heavy_check_mark: void setContent(char *_content) --> set the information the user add into the block .

:heavy_check_mark: int get_unoccupied_index_block() --> find first block that not in use .
:heavy_check_mark: void listAll() --> print all the data from the disk .
:heavy_check_mark: void fsFormat(int blockSize)  --> initializing the disk for the first time considering the block size .
:heavy_check_mark: int CreateFile(const string &fileName) --> create new file on the disk .
:heavy_check_mark: int OpenFile(const string &fileName) --> open the file on open file descriptors for add or read data from .
:heavy_check_mark: string CloseFile(int fd) --> close the file from open file descriptors .
:heavy_check_mark: int WriteToFile(int fd, char *buf, int len)  --> write data for the file when all the data goes into the disk.
:heavy_check_mark: int DelFile(const string &FileName) --> delete the file and all its information from the disk .
:heavy_check_mark: int ReadFromFile(int fd, char *buf, int len) --> read the information of the file from the disk .
:heavy_check_mark: int write(FileDescriptor *fd, char *buf, int len) --> write the information that stay in the block , into the disk.
:heavy_check_mark: int get_index_of_index_block(FileDescriptor *fd) --> get the block index - block that save the indexes of the file data in the disk.
:heavy_check_mark: Block *getIndexBlock(FileDescriptor *fd) --> get index block for the file or make new block for first data .
:heavy_check_mark: Block *getAvailableBlock(FileDescriptor *fd) --> find empty block to store data into .
:heavy_check_mark: int write_block(Block *block) --> write the data that stay in the block into the disk.
:heavy_check_mark: :heavy_check_mark: int del_block(Block *block --> delete all the data from the block .
:heavy_check_mark: Block *read_block(int position) --> read all the data from the block .
:heavy_check_mark: void set_block_occupied(int index_of_data_block) --> set information of the block that we take in the bitVector.
