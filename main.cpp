#include "iostream"
#include "vector"
#include "map"
#include <algorithm>

using namespace std;

struct CodeRLE {
    int len;
    char cur_char;
};

void print_vector(const vector<CodeRLE>& vt) {
    for(auto code : vt)  cout << "(" << code.len << "," << code.cur_char << ")";
    cout << endl;
}

vector<CodeRLE> rle(const string& s) {
    if(s.empty()) {
        return {};
    }

    vector<CodeRLE> result;

    int current_length = 1;
    char previous = s[0];

    for(int q = 1; q < s.size(); q++) {

        char letter = s[q];

        if(letter == previous) {
            current_length++;
            continue;
        }

        result.push_back(CodeRLE{current_length, previous});

        previous = letter;
        current_length = 1;
    }

    result.push_back(CodeRLE{current_length, previous});

    return result;
}

string rle_decode(const vector<CodeRLE>& s) {
    if(s.empty()) {
        return {};
    }

    string result;

    for(auto code: s) {
        for(int q = 0; q < code.len; q++) {
            result += code.cur_char;
        }
    }

    return result;
}

struct Code77 {
    int off;
    int len;
    char next_char;
};

void print_vector(const vector<Code77>& vt) {
    for(auto code : vt)  cout << "(" << code.off << "," << code.len << "," << code.next_char << ")";
    cout << endl;
}

vector<Code77> lz77(const string& s) {
    int max_window_length = 10;

    int search_len, lookup_len;
    vector<Code77> vt;

    for(int i = 0; i < s.length(); i++) {
        search_len = min(i, max_window_length);
        lookup_len = min(search_len, (int)s.length() - i);

        string search_str = s.substr(i - search_len, search_len);
        string lookup_str = s.substr(i, lookup_len);

        int off = -1;

        while(true) {
            if(lookup_len == 0) {
                break;
            }

            string current_str = lookup_str.substr(0, lookup_len);
            off = (int)search_str.find(current_str);

            if(off != -1) {
                break;
            }

            lookup_len--;

            if(lookup_len <= 0) {
                break;
            }
        }

        if(off != -1) {
            Code77 cd{};
            cd.len = lookup_len;
            cd.off = search_len - off;
            cd.next_char = s[i + lookup_len];
            vt.push_back(cd);
            i += lookup_len;
        } else {
            Code77 cd{};
            cd.len = 0;
            cd.off = 0;
            cd.next_char = s[i + lookup_len];
            vt.push_back(cd);
        }
    }

    return vt;
}

string lz77_decode(const vector<Code77>& vt) {
    string result;

    for(auto code : vt) {
        if(code.len == 0) {
            result += code.next_char;
        } else {
            int start = (int)result.length() - code.off;
            string temp = result.substr(start, code.len);
            result += temp + code.next_char;
        }
    }

    return result;
}

struct Code78 {
    int pos;
    char next_char;
};

void print_vector(const vector<Code78>& vt) {
    for(auto code : vt)  cout << "(" << code.pos << "," << code.next_char << ")";
    cout << endl;
}

vector<Code78> lz78(const string& s) {
    string buffer;
    map<string, int> dict;
    vector<Code78> ans;

    for(char i : s) {
        if (dict.find(buffer + i) != dict.end()) {
            buffer += i;
        } else {
            ans.push_back({dict[buffer], i});
            dict[buffer + i] = (int)dict.size() + 1;
            buffer = "";
        }
    }

    if (!buffer.empty()){
        char last_ch = buffer.back();
        buffer.pop_back();
        ans.push_back({dict[buffer], last_ch});
    }

    return ans;
}

string lz78_decode(const vector<Code78>& vt) {
    vector<string> dict = {""};
    string ans;
    
    for (auto code: vt){
        string word = dict[code.pos] + code.next_char;
        ans += word;
        dict.push_back(word);
    }

    return ans;
}

struct ManNode {
    ManNode* left;
    ManNode* right;
    char value;
    int count;
};

struct ManCode {
    string full_code;
    map<char, string> codes;
    ManNode root;
};

bool cmp(ManNode a, ManNode b){
    return a.count < b.count;
}

string getCode(ManNode* root, char value){
    if(root->value == value) {
        return "";
    }

    if(root->left == nullptr && root->right == nullptr) {
        return "null";
    }

    if(root->left != nullptr) {
        string left_ans = getCode(root->left, value);

        if(left_ans != "null") {
            return "1" + left_ans;
        }
    }

    if(root->right != nullptr) {
        string right_ans = getCode(root->right, value);

        if(right_ans != "null") {
            return "0" + right_ans;
        }
    }

    return "null";
}

ManCode sheffman_encode(const string& s) {
    map<char, int> letters;

    for(char c: s) {
        if(letters.find(c) != letters.end()) {
            letters[c] += 1;
        } else {
            letters.insert(make_pair(c, 1));
        }
    }

    vector<ManNode> nodes_list;

    nodes_list.reserve(letters.size());
    for(auto c: letters) {
        nodes_list.push_back({nullptr, nullptr, c.first, c.second});
    }

    while(nodes_list.size() != 1) {
        sort(nodes_list.begin(), nodes_list.end(), cmp);
        ManNode left = nodes_list.front();
        nodes_list.erase(nodes_list.begin());
        ManNode right = nodes_list.front();
        nodes_list.erase(nodes_list.begin());

        ManNode new_node{
            new ManNode{
                    left.left,
                    left.right,
                    left.value,
                    left.count,
                },
            new ManNode{
                    right.left,
                    right.right,
                    right.value,
                    right.count,
                },
            0,
            left.count + right.count
        };
        nodes_list.push_back(new_node);
    }

    ManNode root = nodes_list[0];

    map<char, string> codes;

    for(auto l: letters) {
        codes.insert(make_pair(l.first, getCode(&root, l.first)));
    }

    string code;

    for(char c: s) {
        code += codes.find(c)->second;
    }

    ManCode ans{
            code,
            codes,
            root
    };

    return ans;
}

ManNode *makeTree(const vector<ManNode> &nodes_list, int start, int finish) {
    if(start >= finish) {
        return nullptr;
    }

    if(finish - start == 1) {
        auto *manNode = new ManNode{
            nodes_list[start].left,
            nodes_list[start].right,
            nodes_list[start].value,
            nodes_list[start].count
        };

        return manNode;
    }

    ManNode* left = makeTree(nodes_list, start, (start + finish + 1) / 2);
    ManNode* right = makeTree(nodes_list, (start + finish + 1) / 2, finish);

    return new ManNode{
        left,
        right,
        0,
        left->count + right->count
    };
}

ManCode hoffman_encode(const string& s) {
    map<char, int> letters;

    for(char c: s) {
        if(letters.find(c) != letters.end()) {
            letters[c] += 1;
        } else {
            letters.insert(make_pair(c, 1));
        }
    }

    vector<ManNode> nodes_list;

    nodes_list.reserve(letters.size());
    for(auto c: letters) {
        nodes_list.push_back({nullptr, nullptr, c.first, c.second});
    }

    ManNode root = *makeTree(nodes_list, 0, (int)nodes_list.size());

    map<char, string> codes;

    for(auto l: letters) {
        codes.insert(make_pair(l.first, getCode(&root, l.first)));
    }

    string code;

    for(char c: s) {
        code += codes.find(c)->second;
    }

    ManCode ans{
            code,
            codes,
            root
    };

    return ans;
}

string man_decode(const ManCode& code) {
    string ans;

    ManNode current_node = code.root;

    for(char c: code.full_code) {
        if(current_node.left == nullptr and current_node.right == nullptr) {
            ans += current_node.value;
            current_node = code.root;
        }

        if(c == '1') {
            current_node = *current_node.left;
        } else {
            current_node = *current_node.right;
        }
    }

    if(current_node.left == nullptr and current_node.right == nullptr) {
        ans += current_node.value;
    }

    return ans;
}


void test() {
    string s = "aabbbc";
    cout << s << endl;
    vector<CodeRLE> enc = rle(s);
    print_vector(enc);
    string dec = rle_decode(enc);
    cout << s << endl;
    cout << "Compression: " << (float)enc.size() / (float)s.size() * 2 << endl;

    cout << "----------------------------------" << endl;

    string s77 = "abacabacabadaca";
    cout << s77 << endl;
    vector<Code77> enc77 = lz77(s77);
    print_vector(enc77);
    string dec77 = lz77_decode(enc77);
    cout << dec77 << endl;
    cout << "Compression: " << (float)enc77.size() / (float)s77.size() * 3 << endl;

    cout << "----------------------------------" << endl;

    string s78 = "abacabacabadaca";
    cout << s78 << endl;
    vector<Code78> enc78 = lz78(s78);
    print_vector(enc78);
    string dec78 = lz78_decode(enc78);
    cout << s78 << endl;
    cout << "Compression: " << (float)enc78.size() / (float)s78.size() * 2 << endl;


    cout << "----------------------------------" << endl;

    string ssh = "Эне-бене, рики-таки, \n"
                 "Буль-буль-буль, \n"
                 "Караки-шмаки Эус-деус-краснодеус бац";

    cout << ssh << endl;
    ManCode encsh = sheffman_encode(ssh);
    cout << encsh.full_code << endl;
    string decsh = man_decode(encsh);
    cout << decsh << endl;
    cout << "Compression: " << (float)encsh.full_code.size() / (float)ssh.size() / 8 << endl;

    cout << "----------------------------------" << endl;

    string sho = "Эне-бене, рики-таки, \n"
                 "Буль-буль-буль, \n"
                 "Караки-шмаки Эус-деус-краснодеус бац";

    cout << sho << endl;
    ManCode encho = hoffman_encode(ssh);
    cout << encho.full_code << endl;
    string decho = man_decode(encho);
    cout << decho << endl;
    cout << "Compression: " << (float)encho.full_code.size() / (float)ssh.size() / 8 << endl;
}

void task() {
    string s;

    cout << "Enter string:" << endl;

    cin >> s;

    int encoding;

    vector<CodeRLE> enc;
    vector<Code77> enc77;
    vector<Code78> enc78;
    ManCode encS;
    ManCode encH;

    cout << "Choose encoding:" << endl;
    cout << "0 - RLE" << endl;
    cout << "1 - LZ77" << endl;
    cout << "2 - LZ78" << endl;
    cout << "3 - Sheffman" << endl;
    cout << "4 - Hoffman" << endl;

    cin >> encoding;

    switch (encoding) {
        case 0:
            enc = rle(s);
            print_vector(enc);
            cout << "Compression: " << (float)enc.size() / (float)s.size() * 2 << endl;
            break;
        case 1:
            enc77 = lz77(s);
            print_vector(enc77);
            cout << "Compression: " << (float)enc77.size() / (float)s.size() * 3 << endl;
            break;
        case 2:
            enc78 = lz78(s);
            print_vector(enc78);
            cout << "Compression: " << (float)enc78.size() / (float)s.size() * 2 << endl;
            break;
        case 3:
            encS = sheffman_encode(s);
            cout << encS.full_code << endl;
            cout << "Compression: " << (float)encS.full_code.size() / (float)s.size() / 8 << endl;
            break;
        case 4:
            encH = hoffman_encode(s);
            cout << encH.full_code << endl;
            cout << "Compression: " << (float)encH.full_code.size() / (float)s.size() / 8 << endl;
            break;
        default:
            cout << "Incorrect encoding number" << endl;
            break;
    }
}

int main() {
    system("chcp 65001");

    int n;
    cout << "Choose 0 for test or 1 for task:" << endl;
    cin >> n;

    switch(n){
        case 0:
            test();
            break;
        case 1:
            task();
            break;
        default:
            cout << "There is no such a task" << endl;
            break;
    }

    return 0;
}
