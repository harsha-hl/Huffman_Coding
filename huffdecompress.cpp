#include<iostream> 
#include<unordered_map> 
using namespace std;

string dectobin(int decimal){
    string s="";
    for(int i = 7; i>=0; i--){
        if(decimal>>i & 1)s += '1';
        else s += '0';
    }
    return s;
}

int main(int argc, char *argv[]){
    if(argc != 2){
        cout<<"Invalid cmd line arg. Usage: ./a.out <input file>\n";
        return 1;
    }
    FILE *input = fopen(argv[1], "rb");
    int num_of_unique_chars=0;
    if(input == NULL){
        cout<<"Error opening input file\n";
        return 2;
    }
    string in = argv[1];
    string out = in.substr(0,in.find("-compressed")) + "-decompressed.";
    char buff[1];

    // Read file extension length and the extension itself
    fread(buff, 1, 1, input);
    int fsize = buff[0] - '0';
    while(fsize--){
        fread(buff, 1, 1, input);
        out += buff[0];
    }
    
    FILE *output = fopen(out.c_str(), "w");
    if(output == NULL){
        cout<<"Error creating output file\n";
        return 3;
    }
    unordered_map<string, char> decode_map;
    string s = "";
    int flag=0;

    // Read and fill the hashmap to decode later
    while(fread(buff, 1, 1, input)){
        if(buff[0] != '\0'){
            flag = 0;
            s += buff[0];
        }
        else{
            num_of_unique_chars++;
            flag++;
            if(flag == 2)break;
            decode_map[s.substr(1, s.length()-1)] = s[0];
            s = "";
        }
    }

    // Read and store the padding, read extra null chars 
    fread(buff, 1, 1, input);
    int padding = buff[0] - '0';
    fread(buff, 1, 1, input);  // null
    fread(buff, 1, 1, input);  // null

    // Actual binary code begins from here
    int decimal;
    s = "";
    while(fread(buff, 1, 1, input)){
        // Get decimal(ASCII) equivalent of character
        decimal = buff[0];
        // Append the binary equivalent to s
        s = s + dectobin(decimal);
    }

    // Ignore padding and
    // Convert the binary code back to original text file.
    int start;    
    if(padding == 0){
        s = "0" + s;
        start = 1;
    }
    else start = padding;
    for(int i=1; s[start]!='\0'; i++){
        if(decode_map.find(s.substr(start, i)) != decode_map.end()){
            fwrite(&(decode_map[s.substr(start, i)]), 1, 1, output);
            //cout<<decode_map[s.substr(start, i)];
            start = start + i;
            i = 0;
        }
    }
    cout<<"Decompressed File Succesfully.\n";

    unordered_map<string, char>:: iterator iter;
    for(iter = decode_map.begin(); iter!=decode_map.end(); iter++){
        //cout<<iter->first<<"\t"<<iter->second<<"\n";
    }
    //cout<<"NUMBER OF UNIQUE CHARS - "<<num_of_unique_chars;
    fclose(input);
    fclose(output);
    return 0;
}