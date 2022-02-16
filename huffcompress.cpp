#include <iostream>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <unordered_map>
using namespace std;

#define io                        \
    ios_base::sync_with_stdio(0); \
    cin.tie(0);                   \
    cout.tie(0);

unordered_map<char, string> hashmap;

struct huffnode
{
    char character;
    unsigned int freq;
    struct huffnode *left, *right;
};
typedef struct huffnode *NODE;

struct Minheap
{
    unsigned short size, capacity;
    NODE *array;
};
typedef struct Minheap *HEAP;

NODE newleafnode(char c, int f)
{
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = c;
    tmp->freq = f;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

NODE newinternalnode(int f)
{
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = 0;
    tmp->freq = f;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

void swap(NODE *a, int i, int j)
{
    NODE tmp = (NODE)malloc(sizeof(huffnode));
    tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void heapify(HEAP heap)
{
    int i, j, k;
    unsigned short n = heap->size;
    NODE *a = heap->array;
    for (i = n / 2; i > 0; i--)
    {
        if (2 * i + 1 <= n)
            j = a[2 * i]->freq < a[2 * i + 1]->freq ? 2 * i : 2 * i + 1;
        else
            j = 2 * i;

        if (a[j]->freq < a[i]->freq)
        {
            swap(a, i, j);
            while (j <= n / 2 && (a[j]->freq > a[2 * j]->freq || (2 * j + 1 <= n ? a[j]->freq > a[2 * j + 1]->freq : 0)))
            {
                if (2 * j + 1 <= n)
                    k = a[2 * j]->freq < a[2 * j + 1]->freq ? 2 * j : 2 * j + 1;
                else
                    k = 2 * j;
                swap(a, k, j);
                j = k;
            }
        }
    }
}

NODE mintop(HEAP heap)
{
    NODE top = (NODE)malloc(sizeof(huffnode));
    top = heap->array[1];
    heap->array[1] = heap->array[heap->size--];
    heapify(heap);
    return top;
}

void insertnode(HEAP heap, NODE leftchild, NODE rightchild, int f)
{
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = '\0';
    tmp->freq = f;
    tmp->left = leftchild;
    tmp->right = rightchild;
    heap->array[++heap->size] = tmp;
    heapify(heap);
}

void huffman_tree(HEAP heap)
{
    while (heap->size > 1)
    {
        NODE left_child = mintop(heap);
        NODE right_child = mintop(heap);
        insertnode(heap, left_child, right_child, (left_child->freq + right_child->freq));
    }
}

void assign_codes(NODE root, string str)
{
    string s1 = "", s2 = "";
    if (!root->left && !root->right)
    {
        hashmap[root->character] = str;
    }
    else
    {
        s1 = str + "0";
        s2 = str + "1";
        assign_codes(root->left, s1);
        assign_codes(root->right, s2);
    }
}

void preorder(NODE root)
{
    if (root != NULL)
    {
        cout << root->freq << "\t";
        preorder(root->left);
        preorder(root->right);
    }
}

int main(int argc, char *argv[])
{
    io if (argc != 2)
    {
        cout << "Invalid cmd line arg. Usage: ./a.out <input file>\n";
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        cout << "Error opening input file\n";
        return 2;
    }
    string in = argv[1];
    string filetype = in.substr(in.find(".")+1);
    cout<<filetype<<"\n";
    string out = in.substr(0, in.find(".")) + "-compressed.bin";

    FILE *output = fopen(out.c_str(), "wb");
    if (output == NULL)
    {
        cout << "Error creating output file\n";
        return 3;
    }
    string file_contents = "";
    char buff[1];
    unsigned short num_of_unique_chars = 0;

    // Write file type
    int ftsize = filetype.length();
    char num = ftsize + '0';
    fwrite(&num, 1, 1, output);
    for(int i=0; i<ftsize; i++){
        buff[0] = filetype[i];
        fwrite(buff, 1, 1, output);
    }

    // Holds frequency of each unique character
    int i, f_size = 0, freq[256] = {0};
    while (fread(buff, 1, 1, input))
    {
        if (!freq[buff[0] - 0])
            num_of_unique_chars++;
        freq[buff[0] - 0]++;
        f_size++;
    }
    fclose(input);
    FILE *input1 = fopen(argv[1], "r");

    HEAP heap = (HEAP)malloc(sizeof(struct Minheap));

    // Capacity holds (number of unique characters + 1)
    //cout << "\n\nNUMBER OF UNIQUE CHARS - " << num_of_unique_chars << "\n\n";
    heap->capacity = num_of_unique_chars + 1;
    heap->size = 0;
    heap->array = (NODE *)malloc(heap->capacity * sizeof(struct huffnode));

    for (int i = 0; i < 256; i++)
    {
        if (freq[i])
        {
            heap->size += 1;
            heap->array[heap->size] = newleafnode(i, freq[i]);
        }
    }

    for (int i = 1; i <= heap->size; i++)
    {
        //cout<<heap->array[i]->character<<" - "<<heap->array[i]->freq<<"\n";
    }

    heapify(heap);
    //cout << "Heapify done\n";
    for (int i = 1; i <= heap->size; i++)
    {
        //cout<<heap->array[i]->character<<" - "<<heap->array[i]->freq<<"\n";
    }
    huffman_tree(heap);
    assign_codes(heap->array[heap->size], "");
    unordered_map<char, string>::iterator x;

    //cout << "\nCodes\n";
    for (x = hashmap.begin(); x != hashmap.end(); x++)
        //cout<<x->first<<" - "<<x->second<<'\n';

        // Convert file content to binary code
        // By assigning codes to each character
        while (fread(buff, 1, 1, input))
        {
            file_contents += hashmap[buff[0]];
        }
    //cout<<file_contents.length()<<"\n"<<file_contents<<"\n";
    // Find the padding given to make binary code length a multiple of 8
    int padding = 0;
    if (file_contents.length() % 8 != 0)
        padding = 8 - (file_contents.length() % 8);
    for (int i = 0; i < padding; i++)
    {
        file_contents = "0" + file_contents;
    }

    // Write the codes assigned to characters at the start of the file
    string codes;
    unordered_map<char, string>::iterator iter;
    for (iter = hashmap.begin(); iter != hashmap.end(); iter++)
    {
        codes = "";
        codes += (iter->first + iter->second);
        fwrite(codes.c_str(), codes.length() + 1, 1, output);
        //cout<<iter->first<<"\t"<<iter->second<<"\n";
    }

    // Write the padding given to the binary code
    char null = '\0';
    char pad = padding + '0';
    fwrite(&null, 1, 1, output);
    fwrite(&pad, 1, 1, output);
    fwrite(&null, 1, 1, output);
    fwrite(&null, 1, 1, output);
    // To identify padding with 2 null chars both sides

    // Take 8 bit binary code -> decimal -> character
    // And write this character to ouput file.
    char tmp_bin[9];
    int dec, w;
    //cout<<"\n";
    for (int j = 0; file_contents[j]; j = j + 8)
    {
        strncpy(tmp_bin, &file_contents[j], 8);
        // Convert to integer equivalent and then character
        dec = 0, i = 7;
        while (i > -1)
        {
            w = tmp_bin[i] == '1' ? 1 : 0;
            dec += ((int)pow(2, 7 - i)) * w;
            i--;
        }
        // Print the binary code, its dec equivalent and finally corresponding ascii character.
        //printf("%s\t%d\t%c\n", tmp_bin, dec, dec);
        buff[0] = dec;
        fwrite(buff, 1, 1, output);
    }
    cout<<"Compressed File successfully\n";
    //cout << "NUMBER OF UNIQUE CHARS - " << num_of_unique_chars;
    free(heap);
    fclose(input1);
    fclose(output);
    //cout << "\nEND\n";
    return 0;
}