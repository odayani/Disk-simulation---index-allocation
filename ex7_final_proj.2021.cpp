///Or Dayani - 315710715
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cassert>
#include <cstring>
#include <cmath>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#define DISK_SIZE 256


char decToBinary(int n, char &c) {
    // array to store binary number
    int binaryNum[8];

    // counter for binary array
    int i = 0;
    while (n > 0) {
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }

    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--) {
        if (binaryNum[j] == 1)
            c = c | 1u << j;
    }

    return c;
}


// ============================================================================
class FsFile {

    int file_size; // size of file (num of chars on file)
    int block_in_use; // (optional) - hold the num of the blocks in use
    int index_block; // save the number of blocks who hold the data of the blocks
    int block_size; // num of char we can save on each block

public:

    explicit FsFile(int _block_size) {
        file_size = 0;
        block_in_use = 0;
        block_size = _block_size;
        index_block = -1;
    }

    int getfile_size() const {
        return file_size;
    }

    int get_block_in_use() const {
        return block_in_use;
    }

    int get_index_block() const {
        return index_block;
    }

    int get_block_size() const {
        return block_size;
    }

    void set_index_block(const int _index_block) {
        this->index_block = _index_block;
    }

    void append_file_size(const int _file_size) {
        this->file_size += _file_size;
    }

    void append_block_in_use(const int _block_in_use) {
        this->block_in_use += _block_in_use;
    }

    bool hasBeenWritten() {
        return this->get_index_block() != -1;
    }
};

// ============================================================================

class FileDescriptor {

    string file_name;
    FsFile *fs_file;
    bool inUse;

public:

    FileDescriptor(const string &FileName, FsFile *fsi) {
        file_name = FileName;
        fs_file = fsi;
        inUse = true;
    }

    string getFileName() {
        return file_name;
    }

    bool getInUse() const {
        return this->inUse;
    }

    void setUse(bool temp) {
        this->inUse = temp;
    }

    FsFile *get_Fsfile() {
        return this->fs_file;
    }

};

#define DISK_SIM_FILE "DISK_SIM_FILE.txt"

// ============================================================================
class OpenFileDescriptors {

    FileDescriptor **fileDescriptors{};
    int capacity;

public:

    explicit OpenFileDescriptors(const int block_size) {
        this->capacity = DISK_SIZE / (block_size * block_size);
        this->fileDescriptors = new FileDescriptor *[capacity];
        for (int i = 0; i < capacity; i++) {
            this->fileDescriptors[i] = nullptr;
        }
    }

    int get_size() const {
        return capacity;
    }

    int add(FileDescriptor *fileDescriptor) {
        if (this->fileDescriptors == nullptr) {
            return -1;
        }

        for (int i = 0; i < capacity; i++) {
            if (this->fileDescriptors[i] == nullptr) {
                this->fileDescriptors[i] = fileDescriptor;
                return i;
            }
        }
        return -1;
    }

    void remove_from_openfd(int fd) {
        if (this->fileDescriptors == nullptr) {
            return;
        }
        this->fileDescriptors[fd] = nullptr;
    }

    int delete_fd(FileDescriptor *fileDescriptor) {
        if (this->fileDescriptors == nullptr) {
            return -1;
        }

        for (int i = 0; i < capacity; i++) {
            if (this->fileDescriptors[i] == fileDescriptor) {
                delete fileDescriptor;
                this->fileDescriptors[i] = nullptr;
                return i;
            }
        }
        return -1;
    }

    FileDescriptor *get(int fd) {
        if (this->fileDescriptors == nullptr || fd < 0 || fd >= capacity) {
            return nullptr;
        }
        return this->fileDescriptors[fd];
    }

    int get_by_name(string &filename) {
        if (this->fileDescriptors == nullptr) {
            return -1;
        }

        for (int i = 0; i < capacity; i++) {
            if (this->fileDescriptors[i]->getFileName() == filename) {
                return i;
            }
        }
        return -1;
    }

    int get_by_fd(FileDescriptor *fileDescriptor) {
        if (this->fileDescriptors == nullptr) {
            return -1;
        }

        for (int i = 0; i < capacity; i++) {
            if (this->fileDescriptors[i] == fileDescriptor) {
                return i;
            }
        }
        return -1;
    }

    ~OpenFileDescriptors() { delete this->fileDescriptors; }

};

class Block {

    int index;
    char *content;

public:
    explicit Block(int _index, char *_content) {
        this->index = _index;
        this->content = _content;
    }

    int getIndex() const {
        return index;
    }

    void setIndex(int _index) {
        Block::index = _index;
    }

    char *getContent() const {
        return content;
    }

    void setContent(char *_content) {
        Block::content = _content;
    }

    size_t length() {
        if (this->content == nullptr) return 0;
        return strlen(this->content);
    }

};

class fsDisk {

    FILE *sim_disk_fd;
    bool is_formatted;


    // BitVector - "bit" (int) vector, indicate which block in the disk is free
    // or not. i.e. if BitVector[0] == 1 , means that the first block is occupied.

    int BitVectorSize{};
    int *BitVector{};

    map<string, FileDescriptor *> mainDir;
    OpenFileDescriptors *openFileDescriptor{};

    // MainDir
    // Array that connect between file name to FsFile
    // This array will hold the file name(string) and hold pointer to FsFile both .


    // OpenFileDescriptors
    // Array of all the opened file descriptor
    // The operating system creates an entry to represent that file

    // This entry number is the file descriptor.

    // OpenFileDescriptors;


    int direct_entries;
    int block_size;
    int maxSize{};
    int freeBlocks{};


    // ------------------------------------------------------------------------
public:

    fsDisk() {
        sim_disk_fd = fopen(DISK_SIM_FILE, "r+");
        assert(sim_disk_fd);

        for (int i = 0; i < DISK_SIZE; i++) {
            size_t ret_val = fseek(sim_disk_fd, i, SEEK_SET);
            ret_val = fwrite("\0", 1, 1, sim_disk_fd);
            assert(ret_val == 1);
        }

        fflush(sim_disk_fd);
        direct_entries = 0;
        block_size = 0;
        is_formatted = false;

    }

    // ------------------------------------------------------------------------
    void listAll() {
        int i = 0;
        auto temp = this->mainDir.begin();
        while (temp != mainDir.end()) {
            cout << "index: " << i << ": FileName: " << temp->first << " , isInUse: " << temp->second->getInUse()
            << endl;
            i++;
            temp++;
        }


        char bufy;
        cout << "Disk content: '";
        for (i = 0; i < DISK_SIZE; i++) {
            size_t ret_val = fseek(sim_disk_fd, i, SEEK_SET);
            ret_val = fread(&bufy, 1, 1, sim_disk_fd);
            cout << bufy;
        }

        cout << "'" << endl;

    }


    // ------------------------------------------------------------------------
    void fsFormat(int blockSize = 4) {
        this->block_size = blockSize;
        assert(this->block_size != 0);
        this->is_formatted = true;
        this->openFileDescriptor = new OpenFileDescriptors(blockSize);
        this->BitVectorSize = DISK_SIZE / blockSize;
        this->BitVector = new int[this->BitVectorSize];
        for (int i = 0; i < this->BitVectorSize; i++) {
            this->BitVector[i] = 0;
        }
        this->freeBlocks = BitVectorSize;
    }

    int get_unoccupied_index_block() {
        if (this->BitVector == nullptr) return -1;
        for (int i = 0; i < this->BitVectorSize; i++) {
            if (this->BitVector[i] == 0) {
                return i;
            }
        }
        return -1;
    }

    // ------------------------------------------------------------------------
    int CreateFile(const string &fileName) {
        if (!is_formatted) return -1;
        auto *fsFile = new FsFile(block_size);
        auto *fileDescriptor = new FileDescriptor(fileName, fsFile);
        mainDir.insert(pair<string, FileDescriptor *>(fileName, fileDescriptor));
        size_t ret_val = fwrite(&fileDescriptor, 1, 1, sim_disk_fd);
        assert(ret_val == 1);
        return this->openFileDescriptor == nullptr ? -1 : this->openFileDescriptor->add(fileDescriptor);
    }

    // ------------------------------------------------------------------------
    int OpenFile(const string &fileName) {
        if (this->openFileDescriptor == nullptr) return -1;
        try {
            if (mainDir.find(fileName) == mainDir.end()) {
                return -1;
            }

            FileDescriptor *current = this->mainDir.at(fileName);
            if (current->getInUse()) {
                printf("Already open . \n");
                return this->openFileDescriptor->get_by_fd(current);
            }

            current->setUse(true);
            printf("Open success .\n");
            openFileDescriptor->add(current);
            return this->openFileDescriptor->get_by_fd(current);

        } catch (exception &e) {
            e.what();
            return -1;
        }
    }

    // ------------------------------------------------------------------------
    string CloseFile(int fd) {
        if (this->openFileDescriptor == nullptr) return "-1";
        FileDescriptor *current = openFileDescriptor->get(fd);
        if (current == nullptr) {
            return "-1";
        }
        if (current->getInUse()) {
            current->setUse(false);
            openFileDescriptor->remove_from_openfd(fd);
            return "1";
        }
        return "-1";
    }

    // ------------------------------------------------------------------------
    int WriteToFile(int fd, char *buf, int len) {
        if (!this->is_formatted || this->openFileDescriptor == nullptr || this->BitVector == nullptr) {
            printf("Have to format before write_block. \n");
            return -1;
        }

        if ((freeBlocks - 1) * (block_size) < len) {
            printf("Not enough space on disk. \n");
            return -1;
        }

        if (this->openFileDescriptor->get(fd) == nullptr) {
            printf("File doesn't exist.\n");
            return -1;
        }

        if (!openFileDescriptor->get(fd)->getInUse()) {
            printf("File close, open file before write_block.\n");
            return -1;
        }

        FileDescriptor *fileDescriptor = this->openFileDescriptor->get(fd);
        if (fileDescriptor->get_Fsfile()->getfile_size() + len > this->block_size * this->block_size) {
            printf("Not enough space in the file.\n");
            return -1;
        }

        return write(fileDescriptor, buf, len);
    }

    // ------------------------------------------------------------------------
    int DelFile(const string &FileName) {
        try {
            string temp = FileName;
            if (this->openFileDescriptor == nullptr) return -1;
            FileDescriptor *fileDescriptor = this->mainDir.at(FileName);
            if (fileDescriptor->get_Fsfile()->get_index_block() == -1) {
                int res = openFileDescriptor->get_by_name(temp);
                openFileDescriptor[res].remove_from_openfd(res);
                mainDir.erase(FileName);
                return res;
            }
            Block *index_block = read_block(fileDescriptor->get_Fsfile()->get_index_block(), this->block_size);
            for (int i = 0; i < index_block->length(); i++) {
                int index = (int) (unsigned char) index_block->getContent()[i];
                char data[block_size];
                memset(data, '\0', block_size);
                auto *dummy = new Block(index, data);
                del_block(dummy);
                BitVector[index] = 0;
            }

            char data[block_size];
            memset(data, '\0', block_size);
            index_block->setContent(data);
            del_block(index_block);

            mainDir.erase(FileName);
            BitVector[index_block->getIndex()] = 0;///
            return this->openFileDescriptor->delete_fd(fileDescriptor);
        } catch (const exception &e) {
            e.what();
            return -1;
        }
    }

    // ------------------------------------------------------------------------
    int ReadFromFile(int fd, char *buf, int len) {
        if (this->openFileDescriptor == nullptr) return -1;
        FileDescriptor *fileDescriptor = this->openFileDescriptor->get(fd);
        if (fileDescriptor == nullptr) return -1;
        if (!fileDescriptor->get_Fsfile()->hasBeenWritten()) return -1;
        if (!fileDescriptor->getInUse()) return -1;
        if(len == 0) return 1;
        size_t buf_len = strlen(buf);
        if(buf != nullptr && buf_len > 0) memset(buf, '\0', buf_len);
        Block *index_block = read_block(fileDescriptor->get_Fsfile()->get_index_block(), this->block_size);
        int offset = 0;
        for (int i = 0; i < index_block->length(); i++) {
            int index = (int) (unsigned char) index_block->getContent()[i];
            Block *data = read_block(index, min(this->block_size, len));
            strncpy(buf + offset, data->getContent(), data->length());
            len -= (int) data->length();
            offset += (int) data->length();
            if(len <= 0) break;
        }
        return 1;
    }

private:

    int write(FileDescriptor *fd, char *buf, int len) {
        try {
            int original_length = len, i = 0;
            auto *index_block = getIndexBlock(fd);
            char *index_blocks = new char[this->block_size];
            if (index_block->length() > 0) {
                size_t length = index_block->length();
                strncpy(index_blocks, index_block->getContent(), length);
                i += (int) length;
            }

            set_block_occupied(index_block->getIndex());

            while (len > 0) {
                auto *block = getAvailableBlock(fd);
                if (block->length() != 0) i--;
                index_blocks[i++] = (char) block->getIndex();
                char* data = new char[this->block_size];
                int offset = (int) block->length();
                if (offset > 0) {
                    strncpy(data, block->getContent(), offset);
                }
                int buf_length = (int) strlen(buf);
                strncpy(data + offset, buf, min(buf_length, this->block_size - offset));
                block->setContent(data);
                int written = write_block(block);
                assert(written != 0);
                len -= written - offset;
                buf += written - offset;
                set_block_occupied(block->getIndex());
            }

            fd->get_Fsfile()->set_index_block(index_block->getIndex());
            fd->get_Fsfile()->append_file_size((int) original_length);

            index_block->setContent(index_blocks);
            int written = write_block(index_block);
            assert(written != 0);

            return written;
        } catch (const exception &e) {
            e.what();
            return -1;
        }
    }

    int get_index_of_index_block(FileDescriptor *fd) {
        return fd->get_Fsfile()->hasBeenWritten() ? fd->get_Fsfile()->get_index_block()
        : get_unoccupied_index_block();
    }

    Block *getIndexBlock(FileDescriptor *fd) {
        if (fd->get_Fsfile()->hasBeenWritten()) {
            return read_block(fd->get_Fsfile()->get_index_block(), this->block_size);
        }
        int index_of_index_block = get_index_of_index_block(fd);
        return new Block(index_of_index_block, nullptr);
    }

    Block *getAvailableBlock(FileDescriptor *fd) {
        if (fd->get_Fsfile()->hasBeenWritten()) {
            Block *index_block = read_block(fd->get_Fsfile()->get_index_block(), this->block_size);
            int index_of_last_block = (int) (unsigned char) index_block->getContent()[index_block->length() - 1];
            Block *last_block = read_block(index_of_last_block, this->block_size);
            if (last_block->length() >= this->block_size) {
                return new Block(get_unoccupied_index_block(), nullptr);
            }
            return last_block;
        }

        int index = get_unoccupied_index_block();
        return new Block(index, nullptr);
    }

    int write_block(Block *block) {
        int cursor = fseek(this->sim_disk_fd, block->getIndex() * block_size, SEEK_SET);
        assert(cursor == 0);
        char *current = block->getContent();
        size_t length = strlen(current);
        int written = (int) fwrite(current, sizeof(char), length, this->sim_disk_fd);
        assert(written != 0);
        return written;
    }

    int del_block(Block *block) {
        int cursor = fseek(this->sim_disk_fd, block->getIndex() * block_size, SEEK_SET);
        assert(cursor == 0);
        int written = (int) fwrite(block->getContent(), sizeof(char), sizeof(block->getContent()), this->sim_disk_fd);
        assert(written != 0);
        return written;
    }

    Block *read_block(int position, int len) {
        int cursor = fseek(this->sim_disk_fd, position * block_size, SEEK_SET);
        assert(cursor == 0);
        char *data = new char[this->block_size];
        size_t read_ret = fread(data, len, 1, this->sim_disk_fd);
        assert(read_ret != 0);
        auto *block = new Block(position, data);
        return block;
    }

    void set_block_occupied(int index_of_data_block) {
        if (this->BitVector == nullptr) throw exception();
        BitVector[index_of_data_block] = 1;
        freeBlocks--;
    }
};


int main() {
    int blockSize;
    int direct_entries;
    string fileName;
    char str_to_write[DISK_SIZE];
    char str_to_read[DISK_SIZE];
    int size_to_read;
    int _fd;

    auto *fs = new fsDisk();
    int cmd_;
    while (true) {
        cin >> cmd_;
        switch (cmd_) {
            case 0:   // exit
            delete fs;
            exit(0);

            case 1:  // list-file
            fs->listAll();
            break;

            case 2:    // format
            cin >> blockSize;
            fs->fsFormat(blockSize);
            break;

            case 3:    // create-file
            cin >> fileName;
            _fd = fs->CreateFile(fileName);
            cout << "CreateFile: " << fileName << " with File Descriptor #: " << _fd << endl;
            break;

            case 4:  // open-file
            cin >> fileName;
            _fd = fs->OpenFile(fileName);
            if (_fd == -1) {
                printf("File not found .\n");
                break;
            }
            cout << "OpenFile: " << fileName << " with File Descriptor #: " << _fd << endl;
            break;

            case 5:  // close-file
            cin >> _fd;
            fileName = fs->CloseFile(_fd);
            if (fileName == "-1") {
                printf("File already closed .\n");
                break;
            }
            cout << "CloseFile: " << fileName << " with File Descriptor #: " << _fd << endl;
            break;

            case 6:   // write_block-file
            cin >> _fd;
            cin >> str_to_write;
            fs->WriteToFile(_fd, str_to_write, (int) strlen(str_to_write));
            break;

            case 7:    // read_block-file
            cin >> _fd;
            cin >> size_to_read;
            fs->ReadFromFile(_fd, str_to_read, size_to_read);
            cout << "ReadFromFile: " << str_to_read << endl;
            break;

            case 8:   // delete file
            cin >> fileName;
            _fd = fs->DelFile(fileName);
            cout << "DeletedFile: " << fileName << " with File Descriptor #: " << _fd << endl;
            break;
            default:
                break;
        }
    }
}