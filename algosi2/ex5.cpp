//
// Created by moira-q on 25.04.20.
//

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <bitset>

typedef unsigned char byte;

struct IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

struct IOutputStream {
    virtual void Write(byte value) = 0;
};

class VectorInput : public IInputStream {
public:
    VectorInput(std::vector<byte>* vec) : vec_ptr(vec), pos(0) {
    }

    virtual bool Read(byte& value) override {
        if (pos >= vec_ptr->size()) {
            return false;
        }
        value = (*vec_ptr)[pos++];
        return true;
    }
private:
    std::vector<byte>* vec_ptr;
    int pos;
};

class VectorOutput : public IOutputStream {
public:
    VectorOutput(std::vector<byte>* vec) : vec_ptr(vec) {
    }

    virtual void Write(byte value) override {
        vec_ptr->push_back(value);
    }

private:
    std::vector<byte>* vec_ptr;
};


class BitInputStream {
public:
    BitInputStream(IInputStream& input) : input_stream(input), bit_pos(0) {
    }

    bool read_bit(byte& res) {
        if (bit_pos == 0) {
            bool input_res = input_stream.Read(buf);
            if (!input_res) {
                return false;
            }
        }
        res = (buf >> (bit_pos++)) & static_cast<byte >(1);
        if (bit_pos == 8) {
            bit_pos = 0;
        }
        return true;
    }

    byte read_byte() {
        byte result = 0;
        byte buff = 0;
        for (byte i = 0; i < 8; ++i) {
            read_bit(buff);
            result = result | (buff << i);
        }
        return result;
    }
private:
    IInputStream& input_stream;

    byte buf;
    int bit_pos;
};

class BitOutputStream {
public:
    BitOutputStream(IOutputStream& output) : output_stream(output), buf(0), bit_pos(0) {
    }

    void write_bit(byte bit) {
        if (bit_pos == 8) {
            flush();
        }
        buf = buf | ((bit & static_cast<byte >(1)) << (bit_pos++));
    }

    void write_byte(byte wbyte) {
        for (byte i = 0; i < 8; ++i) {
            write_bit(wbyte & static_cast<byte >(1));
            wbyte = wbyte >> 1;
        }
    }

    void flush() {
        output_stream.Write(buf);
        buf = 0;
        bit_pos = 0;
    }
private:
    IOutputStream& output_stream;
    byte buf;
    int bit_pos;
};


// Метод архивирует данные из потока original
//void Encode(IInputStream& original, IOutputStream& compressed);
// Метод восстанавливает оригинальные данные
//void Decode(IInputStream& compressed, IOutputStream& original);


class Haffman {
public:
    Haffman(IInputStream& in, IOutputStream& out): input(in), output(out), Haffmantree(nullptr) {}
    void SetBase() {
        byte buff;
        while (input.Read(buff)) {
            text.push_back(buff);
            counter[buff]++;
        }
        std::set<std::pair<int, byte>> sorted_counter;
        for (auto n: counter) {
            sorted_counter.insert({n.second, n.first});
        }
        for (auto value: sorted_counter) {
            AddToHaffmantree(value.second);
        }
        CodedSymbol cs;
        FillCodeTable(Haffmantree, cs);
    }
    void FillEncodeOutput() {
        int tree_size = (counter.size() * 8) + (code_table.size() * 2) - 1;
        int coded_text_size = 0;
        for (const auto& icounter: counter) {
            coded_text_size += code_table[icounter.first].bits_count * icounter.second;
        }
        coded_text_size += tree_size + 3;
        bool need_code = (((text.size()) * 8) > (coded_text_size + 8));
        if (need_code) {
            BitOutputStream coded(output);
            coded.write_bit(1);
            uint8_t table_count = code_table.size();
            coded.write_byte(table_count);
            int unused_bit = (coded_text_size + 1) % 8;
            unused_bit = (8 - unused_bit) % 8;
            for (int i = 0; i < 3; ++i) {
                coded.write_bit(unused_bit & static_cast<byte >(1));
                unused_bit = unused_bit >> 1;
            }

            tree_write(Haffmantree, coded);
            for (const auto &c: text) {
                WriteCodedSymbol(code_table[c], coded);
            }
            coded.flush();
        } else {
            BitOutputStream coded(output);
            coded.write_bit(0);
            for (const auto &c: text) {
                coded.write_byte(c);
            }
        }
    }
    void FillDecodeOutput() {
        BitInputStream coded(input);
        byte need_decode = 0;
        coded.read_bit(need_decode);
        if (need_decode & 1) {
            byte alph_size = coded.read_byte();
            byte unused_bit = 0;
            for (int i = 0; i < 3; ++i) {
                byte bit;
                coded.read_bit(bit);
                bit = bit << i;
                unused_bit = unused_bit | bit;
            }
            Haffmantree = tree_read(coded);
            int buff_size = unused_bit / (alph_size - 1);
            std::queue<byte> buffer;
            byte byte_buff = 0;
            //BitOutputStream deced
            while (GetEcodedSymbol(coded, byte_buff, Haffmantree)) {
                if (buffer.size() < buff_size) {
                    buffer.push(byte_buff);
                } else {
                    if (buff_size == 0) {
                        output.Write(byte_buff);
                    } else {
                        output.Write(buffer.front());
                        buffer.pop();
                        buffer.push(byte_buff);
                    }
                }
            }
        } else {
            byte byte_buff = 0;
            while(GetNonCodeSymbol(coded, byte_buff)) {
                output.Write(byte_buff);
            }
            if (byte_buff != 0) {
                output.Write(byte_buff);
            }
        }

    }
private:
    struct Node {
        Node* left;
        Node* right;

        byte value;
        Node() : left(nullptr), right(nullptr), value(0) {}
        Node(byte b) : left(nullptr), right(nullptr), value(b) {}
    };
    struct CodedSymbol {
        uint8_t code;
        uint8_t bits_count;
        CodedSymbol(): code(0), bits_count(0) {}
    };

    IInputStream& input;
    IOutputStream& output;
    std::vector<byte> text;
    std::map<byte, int> counter;
    std::unordered_map<byte, CodedSymbol> code_table;
    Node* Haffmantree;

    void WriteCodedSymbol(CodedSymbol cd, BitOutputStream& ss) {
        for (int i = 0; i < cd.bits_count - 1; ++i) {
            ss.write_bit(0);
        }
        ss.write_bit(cd.code);
    }
    bool GetNonCodeSymbol(BitInputStream& ss, byte& byte_buff) {
        byte result = 0;
        byte buff = 0;
        for (byte i = 0; i < 8; ++i) {
            if(!ss.read_bit(buff)) {
                byte_buff = result;
                return false;
            }
            result = result | (buff << i);
        }
        byte_buff = result;
        return true;
    }
    bool GetEcodedSymbol(BitInputStream& ss, byte& byte_buff, Node* head ) {
        if (!head->right && !head->left) {
            byte_buff = head->value;
            return true;
        }
        byte bit = 0;
        if (ss.read_bit(bit)) {
            if (bit == 0) {
                return GetEcodedSymbol(ss, byte_buff, head->left);
            } else {
                return GetEcodedSymbol(ss, byte_buff, head->right);
            }
        } else {
            return false;
        }
    }

    void tree_write(Node* node, BitOutputStream& ss) {
        if (!node) {
            return;
        }
        if (!node->left && !node->right) {
            ss.write_bit(0);
            ss.write_byte(node->value);
        } else {
            ss.write_bit(1);
            tree_write(node->left, ss);
            tree_write(node->right, ss);
        }
    }
    Node* tree_read(BitInputStream& ss) {
        Node* node = new Node();
        byte bit = 0;
        ss.read_bit(bit);
        if (bit == 0) {
            node->value = ss.read_byte();
        } else {
            node->left = tree_read(ss);
            node->right = tree_read(ss);
        }
        return node;
    }

    void FillCodeTable(Node* head, CodedSymbol& cs) {
        if (!head->left && !head->right) {
            code_table[head->value] = cs;
            return;
        }
        CodedSymbol l_cs = cs;
        l_cs.code = (l_cs.code << static_cast<byte >(1)) & static_cast<byte >(0);
        l_cs.bits_count++;
        FillCodeTable(head->left, l_cs);

        CodedSymbol r_cs = cs;
        r_cs.code = (r_cs.code << static_cast<byte >(1)) | static_cast<byte >(1);
        r_cs.bits_count++;
        FillCodeTable(head->right, r_cs);
    }

    void AddToHaffmantree(byte new_value) {
        if (!Haffmantree) {
            Haffmantree = new Node(new_value);
            return;
        }
        Node* new_root = new Node(0);
        Node* insert_node = new Node(new_value);
        Node* buff = Haffmantree;
        Haffmantree = new_root;
        Haffmantree->left = buff;
        Haffmantree->right = insert_node;

    }

};

void Encode(IInputStream& original, IOutputStream& compressed) {
    Haffman hf(original, compressed);
    hf.SetBase();
    hf.FillEncodeOutput();

//    copyStream(original, compressed);

}

void Decode(IInputStream& compressed, IOutputStream& original) {
    Haffman hf(compressed, original);
    hf.FillDecodeOutput();
    //  copyStream(compressed, original);
}

int main(int argc, const char * argv[]) {


    std::string str = "qwerty";
    std::vector<byte> vec;
    for (auto& c: str) {
        vec.push_back(c);
    }
/*    for (int k = 32; k < 127; ++k) {
        vec.push_back(static_cast<char>(k));
    }*/
    VectorInput vec_input(&vec);

    std::vector<byte> vec1;
    VectorOutput vec_output(&vec1);

    Encode(vec_input, vec_output);

    std::vector<byte> vec12;

    VectorInput vec_in(&vec1);
    VectorOutput vec_out(&vec12);


    for (int i = 0; i < vec1.size(); i++) {
        std::bitset<8> bs = (int) vec1[i];
        std::cout << (int) vec1[i] << " => " << bs.to_string() << std::endl;
    }

    Decode(vec_in, vec_out);
    for (int j = 0; j < vec12.size(); ++j) {
        std::cout << vec12[j];
    }

        return 0;

}