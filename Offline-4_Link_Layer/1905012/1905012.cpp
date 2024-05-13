
#include<bits/stdc++.h>
#include <windows.h>
using namespace std;
#define ll long long
#define N 10005


string initial_str;
int m;      // number of bytes in a data block
double p; // probability that a bit will toggle
string polynomial;
string str;


// additional
int rows;
int bits_per_row;
int total_check_bits;
vector<int>check_bit_pos;
vector<int>sending_bits;
vector<int>receiving_bits;

vector<int>row[N];
vector<int>received_row[N];
map<int,int>mp;

vector<int>ans;
vector<int>poly;

// take input
void take_input()
{
    cout<<"enter data string: ";
    cout.flush();
    getline(cin, initial_str);
    cout<<"enter number of data bytes in a row (m): ";
    cout.flush();
    cin>>m;
    cout<<"enter probability (p): ";
    cout.flush();
    cin>>p;
    cout<<"enter generator polynomial: ";
    cout.flush();
    cin>>polynomial;
}

//  1.If the size of the data string is not a multiple of m,
//  append the padding character (~) to the data string accordingly.
//  Print the updated data string.

void do_padding()
{
    int len=initial_str.size();
    str=initial_str;
    if(len%m!=0)                        // not multiple of m
    {
        int rem=((len+m-1)/m)*m-len;
        for(int i=1; i<=rem; i++)
        {
            str+="~";                   // adding padding character
        }
    }
    cout<<"\n\n";
    cout<<"data string after padding: "<<str<<'\n';
}


// 2. Create the data block, which contains the 8-bit binary representation of the ASCII codes
// of the characters of the data string.
// Each row contains ASCII codes of m characters.
// The first row shows the first m characters, the second row shows the next m characters, and so on.
// Print the data block. Note that, there will be l/m rows in the data block,
// where l is the length of the padded data string.

void get_row(int row_num)
{
    int startCharPos=(row_num)*m;
    for(int i=startCharPos; i<(row_num+1)*m; i++)
    {
        char now=str[i];
        int here=now;
        for(int j=7; j>=0; j--)
        {
            int res=(1<<j)&here;        // bit masking j-th bit
            if(res!=0)
                row[row_num].push_back(1);
            else
                row[row_num].push_back(0);
        }
    }
}
void print_row(int row_num)
{
    for(int i=0; i<row[row_num].size(); i++)
    {
        cout<<row[row_num][i];
    }
    cout<<endl;
}
void get_rows()
{
    cout<<'\n';
    cout<<"data block <ascii code of m characters per row>:";
    cout<<'\n';
    rows=str.size()/m;  // number of rows in the data block = (size of the padded string / m)
    for(int i=0; i<rows; i++)
    {
        get_row(i);     // get bit stream per row
        print_row(i);   // print that row
    }
}


//  GETTING CHECK BITS
void get_check_bits()
{
    bits_per_row=(m*8);
    int total=0;
    int base=2;
    int startPos=1;
    for(int i=1; i<=1e5; i++)
    {
        if(total>bits_per_row)
            break;
        check_bit_pos.push_back(startPos);
        int prevPos=startPos;
        startPos=startPos*base;
        total+=((startPos-1)-(prevPos));
    }
    total_check_bits=check_bit_pos.size();
    /*for(int i=0; i<check_bit_pos.size(); i++)
    {
        cout<<check_bit_pos[i]<<endl;
    }*/
}


//  3. Add check bits to correct at most one-bit error in each row of the data block (according to page 195, figure 3-7 of [1]).
//  Print the updated data block. Note that, the check bits must be shown in a different color (green) [2].

void insert_check_bits_row(int row_num)
{
    vector<int>temp=row[row_num];       // temp now contains the data bits without check bits
    row[row_num].clear();               // here final row will be updated
    mp.clear();
    int total_len=check_bit_pos.size()+bits_per_row; // total bits = data bits + check bits
    map<int,int>marked_check_bits;      // to access the check bits in O(1) time complexity
    for(int i=0; i<check_bit_pos.size(); i++)
    {
        int now=check_bit_pos[i];
        marked_check_bits[now]=1;   // marked true that it is a check bit
    }
    int start=0;    // so far how many data bits are moved to the final row[row-number]
    for(int i=0; i<=total_len; i++)
    {
        if(i==0 || marked_check_bits[i]==1)
        {
            row[row_num].push_back(-1); // 1 based indexing, initially the check bits are -1 (placeholder)
            continue;
        }
        row[row_num].push_back(temp[start]);
        int here=temp[start];
        start++;
        // CHECKING WHICH CHECK BIT POSITIONS THIS DATA BIT WILL CONTRIBUTE TO
        for(int j=0; j<32; j++)
        {
            ll now=(1LL<<j & i );
            if(now!=0)  // the data bit will contribute to this check bit position
            {
                int here=mp[now];
                here=here^row[row_num][i];
                mp[now]=here;
            }
        }
    }
    // done with calculating check bits and moved the data bits to the final row
    for(int i=0; i<=total_len; i++)
    {
        if(i==0)
            continue;
        if(marked_check_bits[i]==1)
        {
            int pos=i;
            int val=mp[pos];    // the value of that check bit now
            row[row_num][pos]=val;
        }
    }
    // done with calculating the final row
    for(int i=1; i<row[row_num].size(); i++)
    {
        if(marked_check_bits[i]==1)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            cout<<row[row_num][i];
            cout.flush();
        }
        else
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            cout<<row[row_num][i];
            cout.flush();
        }
    }
    cout<<endl;
}

void insert_check_bits()
{
    cout<<'\n';
    cout<<"data block after adding check bits:"<<endl;
    rows=str.size()/m;
    for(int i=0; i<rows; i++)
    {
        insert_check_bits_row(i);
    }
}

// 4. Serialize the above data block in column-major manner.
// Print the serialized data bits.

void serialize_column_major()
{
    for(int j=1; j<row[0].size(); j++)
    {
        for(int i=0; i<rows; i++)
        {
            sending_bits.push_back(row[i][j]);
        }
    }
    cout<<"\n";
    cout<<"data bits after column-wise serialization:"<<endl;
    for(int i=0; i<sending_bits.size(); i++)
    {
        cout<<sending_bits[i];
    }
    cout<<endl;
}


// 5.Compute the CRC checksum of the above bit stream using the generator polynomial (according to page 197-198, figure 3-8 of [1]).
// Append the checksum to the bit stream. This is the frame to be transmitted.
// Print the frame. Note that, the appended checksum must be shown in a different color (cyan).

void crc_calc()
{
    vector<int>target=ans;
    ans.clear();
    if(target.size()<poly.size())
    {
        ans=target;
        return;
    }
    else if(target.size()==poly.size())
    {
        vector<int>temp;
        int start=-1;
        for(int i=0; i<poly.size(); i++)
        {
            int now=target[i]^poly[i];
            //cout<<"index "<<i<<" "<<target[i]<<" "<<poly[i]<<endl;
            if(now==1 && start==-1)
            {
                start=i;// book keeping the first 1
            }
            temp.push_back(now);
        }

        if(start!=-1)
        {
            for(int i=start; i<temp.size(); i++)
            {
                ans.push_back(temp[i]);
            }
        }
    }
    else
    {
        cout<<"BUG HERE "<<endl;
    }
}

void crc()
{
    // character to int stream of the polynomial
    for(int i=0; i<polynomial.size(); i++)
    {
        int here=polynomial[i]-'0';
        poly.push_back(here);
    }
    // how many additional checksum bits will be added
    int additionals=polynomial.size();
    additionals--;
    for(int i=1; i<=additionals; i++)
    {
        sending_bits.push_back(0);
    }

    int start=-1;
    for(int i=0; i<sending_bits.size(); i++)
    {
        if(sending_bits[i]==1) // 0 continued, then will not be added
        {
            start=i;
        }
        if(start!=-1)
        {
            ans.push_back(sending_bits[i]);
            crc_calc();
            if(ans.empty()==true)
            {
                start=-1;
            }
            else
            {

            }
        }
    }
    if(ans.empty()==true)
        return;         // nothing to do, or nothing to overwrite on the 0 added
    // ans vector must be or with last additional 0  bits (checksum)

    vector<int>temp_ans;
    int gap=abs((int)ans.size()-additionals); // how many 0s are there before leading 1 in the checksum
    for(int i=1;i<=gap;i++)
        temp_ans.push_back(0);
    for(int i=0;i<ans.size();i++)
    {
        temp_ans.push_back(ans[i]);
    }
    for(int i=0;i<temp_ans.size();i++)
    {
        sending_bits.pop_back(); // pop the 0s
    }
    for(int i=0;i<temp_ans.size();i++)
    {
        sending_bits.push_back(temp_ans[i]); // push the final checksum
    }
    cout<<'\n';
    cout<<"data bits after appending CRC checksum <sent frame>:"<<endl;
    for(int i=0;i<sending_bits.size();i++)
    {
        if(i>=(sending_bits.size()-temp_ans.size()))
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
            cout<<sending_bits[i];
            cout.flush();
        }
        else
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            cout<<sending_bits[i];
            cout.flush();
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout<<endl;
}

// 6.Simulate the physical transmission by toggling each bit of the stream with a probability of p.
// Print the received frame. Note that, the erroneous bits must be shown in a different color (red).

void receive_bits()
{
    cout<<'\n';
    srand( (unsigned)time( NULL ) );
    for(int i=0;i<sending_bits.size();i++)
    {
        int now=sending_bits[i];
        double now_p=(float) rand()/RAND_MAX;
        //cout<<now_p<<endl;
        if(now_p<=p)
        {
            now=now^1; //including error
        }
        receiving_bits.push_back(now);
    }
    cout<<"received frame: "<<endl;
    for(int i=0;i<receiving_bits.size();i++)
    {
        if(sending_bits[i]!=receiving_bits[i])
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        }
        else
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        }
        cout<<receiving_bits[i];
        cout.flush();
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout<<endl;
}


// 7.Verify the correctness of the received frame using the generator polynomial (according to page 198 of [1]).
// Print the result of the error detection procedure.
void crc_receive()
{
    cout<<"\n";
    ans.clear();
    int start=-1;
    for(int i=0; i<receiving_bits.size(); i++)
    {
        if(receiving_bits[i]==1 && start==-1)
        {
            start=i;
        }
        if(start!=-1)
        {
            ans.push_back(receiving_bits[i]);
            crc_calc();
            if(ans.empty()==true)
            {
                start=-1;
            }
        }
    }
    if(ans.empty()==true)
        {
            cout<<"result of CRC checksum matching: no error detected"<<endl;
        }
        else
        {
            cout<<"result of CRC checksum matching: error detected"<<endl;
        }
}

// 8.Remove the CRC checksum bits from the data stream and de-serialize it
// into the data-block in a column-major fashion.
// Print the data block. Note that, the erroneous bits must be shown in a different color (red).
void remove_crc()
{
    int additionals=poly.size()-1;
    for(int i=1;i<=additionals;i++)
    {
        receiving_bits.pop_back();
    }
    int start=0;

    for(int i=0;i<rows;i++)
    {
        received_row[i].push_back(-1);
    }

    for(int j=1; j<row[0].size(); j++)
    {
        for(int i=0; i<rows; i++)
        {
            received_row[i].push_back(receiving_bits[start]);
            start++;
        }
    }
    cout<<'\n';
    cout<<"data block after removing CRC checksum bits: "<<endl;
    for(int i=0;i<rows;i++)
    {
        for(int j=1;j<received_row[i].size();j++)
        {
            if(row[i][j]!=received_row[i][j])
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            }
            cout<<received_row[i][j];
            cout.flush();
        }
        cout<<endl;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

//  9.Correct the error in each row according to the method described in page 195 of [1].
//  Observe that, if there is more than one error in a row, this error correction mechanism will fail to correct the error.
//  Print the data block after correcting the errors and removing the check bits.
void remove_check_bits_row(int row_num)
{
    vector<int>temp=received_row[row_num];       // temp now contains the data bits without check bits
    //received_row[row_num].clear();               // here final row will be updated
    mp.clear();
    int total_len=check_bit_pos.size()+bits_per_row; // total bits = data bits + check bits
    map<int,int>marked_check_bits;      // to access the check bits in O(1) time complexity
    for(int i=0; i<check_bit_pos.size(); i++)
    {
        int now=check_bit_pos[i];
        marked_check_bits[now]=1;   // marked true that it is a check bit
        mp[now]=0;
    }
    int start=0;    // so far how many data bits are moved to the final row[row-number]
    for(int i=0; i<=total_len; i++)
    {
        if(i==0 || marked_check_bits[i]==1)
            continue;
        for(int j=0; j<32; j++)
        {
            ll now=(1LL<<j & i );
            if(now!=0)  // the data bit will contribute to this check bit position
            {
                int here=mp[now];
                here=here^received_row[row_num][i];
                mp[now]=here;
            }
        }
    }

    // done with calculating check bits and moved the data bits to the final row
    int error_pos=0;
    for(int i=0; i<=total_len; i++)
    {
        if(i==0)
            continue;
        if(marked_check_bits[i]==1)
        {
            int pos=i;
            int val=mp[pos];    // the value of that check bit now
            if(val!=row[row_num][pos])
            {
                error_pos+=pos;
            }
        }
    }
    //cout<<row_num<<" "<<error_pos<<endl;
    if(error_pos!=0 && error_pos<received_row[row_num].size())
    {
        received_row[row_num][error_pos]=received_row[row_num][error_pos]^1;//corrected
    }

    // done with calculating the final row
    for(int i=1; i<received_row[row_num].size(); i++)
    {
        if(marked_check_bits[i]==1)
        {
            continue;
        }
        else
        {
            cout<<received_row[row_num][i];
        }
    }
    cout<<endl;
}

void remove_check_bits()
{
    cout<<'\n';
    cout<<"data block after removing check bits:"<<endl;
    for(int i=0; i<rows; i++)
    {
        remove_check_bits_row(i);
    }
}
//  10.	From the bits of the data block, compute the ASCII codes of the characters.
//  Print the data string.

void getFrame()
{
    cout<<"\n";
    cout<<"output frame: ";
    cout.flush();
    vector<char>ans;
    map<int,int>marked_check_bits;
    for(int i=0; i<check_bit_pos.size(); i++)
    {
        int now=check_bit_pos[i];
        marked_check_bits[now]=1;
    }
    int start_temp=1;
    for(int i=0;i<rows;i++)
    {
        vector<int>temp[1000];
        for(int j=1;j<received_row[i].size();j++)
        {
            if(marked_check_bits[j]==1)
                continue;
            int now=received_row[i][j];
            if(temp[start_temp].empty()==true || temp[start_temp].size()<8)
            {
                temp[start_temp].push_back(now);
            }
            else
            {
                start_temp++;
                temp[start_temp].push_back(now);
            }
        }
        int total[100];
        start_temp=1;
        for(int i=1;i<=m;i++) // m ta output hobe
        {
            int total=0;
            for(int j=0;j<temp[i].size();j++)
            {
                total=(total<<1)|temp[i][j];
            }
            ans.push_back((char)(total));
        }
    }
    for(int i=0;i<ans.size();i++)
    {
        cout<<ans[i];
    }
    cout<<endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    take_input();                   // input taken
    do_padding();                   // TASK-1
    get_rows();                     // TASK-2
    get_check_bits();               // for getting which check bits are needed according to the equation (m+r+1)<=2^r
    insert_check_bits();            // TASK-3
    serialize_column_major();       // TASK-4
    crc();                          // TASK-5
    receive_bits();                 // TASK-6
    crc_receive();                  // TASK-7
    remove_crc();                   // TASK-8
    remove_check_bits();            // TASK-9
    getFrame();                     // TASK-10
    return 0;
}

