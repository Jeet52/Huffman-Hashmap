# Huffman File Compression with Hashmap

This project implements **Huffman Encoding** using a custom-built `hashmap` class and applies it to compress and decompress text files. It includes all key components of the Huffman algorithm: frequency analysis, encoding tree construction, binary encoding, and decompression logic. The project was built as part of **CS 251: Data Structures** at UIC, Summer 2024.

---

## 📁 Project Structure

```
.
├── hashmap.cpp        # Custom hashmap implementation
├── hashmap.h
├── util.h             # Huffman logic: encoding, decoding, compression
├── bitstream.h        # Stream wrapper for reading/writing bits
├── main.cpp           # (Optional) main file for testing compress/decompress
├── README.md
└── LICENSE
```

---

## 🚀 Features

- Efficient frequency counting using a custom hashmap
- Priority queue-based Huffman tree construction
- Binary encoding using a map of character → bit string
- Encoding/decoding of text files
- File compression to `.huf` format
- Recovery of original file using `.huf` decompression

---

## 🛠 How to Compile & Run

> 📌 Requires a C++ compiler that supports C++11 or later (e.g. `g++`, `clang++`)

### Compile:
```bash
g++ hashmap.cpp main.cpp -o huffman
```

### Run Compression:
```bash
./huffman compress input.txt
# Creates output file: input.txt.huf
```

### Run Decompression:
```bash
./huffman decompress input.txt.huf
# Creates output file: input_unc.txt
```

---

## 🧪 Sample Workflow

### Input (`input.txt`)
```
this is an example of a huffman tree
```

### Compressed (`input.txt.huf`)
- Binary encoded using Huffman tree
- Includes frequency map in file header

### Decompressed (`input_unc.txt`)
```
this is an example of a huffman tree
```

✅ **Lossless recovery** verified.

---

## 📚 Key Concepts Used

- Custom Hashmap (with chaining)
- Priority Queue (`std::priority_queue`)
- Recursion & Tree Traversals
- File I/O & Bit Manipulation
- Operator Overloading (`<<`, `>>`)

---

## 🧑‍💻 Author

**Jeet Patel**  
CS 251, Summer 2024  
Instructor: Scott Reckinger  
University of Illinois Chicago (UIC)

---

## 📄 License

This project is open-source under the MIT License. See [LICENSE](./LICENSE) for details.
