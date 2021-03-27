//
//  main.cpp
//  OneBanc
//
//  Created by Pawan Kumar on 27/03/21.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

// GetFileExtension -----------------
void GetFileExtension(const string &inputFile, string &extension)
{
    // Find last dot in string
    int len = inputFile.length();
    int i = 0;
    for(i = len-1; i >= 0; i--)
        if(inputFile[i] == '.')break;
    
    for(int j = i+1; j < len; j++)
        extension += inputFile[j];
}

// IsValidFileFormat ---------------
bool IsValidFileFormat(const string &inputFile)
{
    bool bIsValid = true;
    // Get File extension
    string extension = "";
    GetFileExtension(inputFile,extension);
    if(extension != "csv")bIsValid = false;
    return bIsValid;
}

// Get OutPut File Name ------------------
void GetOutPutFileName(const string &inputFile, string &outputFile)
{
    outputFile = inputFile;
    // Find first '-'
    int i = 0;
    int len = outputFile.length();
    while(i < len)
    {
        if(outputFile[i] == '-')break;
        i++;
    }
    // Replcae Input by OutPut
    outputFile.replace(i+1,5,"Output");
}

//Read File

void Read_File(const string & inputFile, vector<vector<string>> &strListLines, vector<int> &nArrEffWords)
{
    // Open current csv file
    fstream read_file;
    read_file.open(inputFile, ios::in);
    string curr_line,temp_line;
    while(getline(read_file,curr_line))
    {
        if(curr_line.empty())continue;
        
        stringstream ss(curr_line);
        vector<string> curr_line_words;
        string curr_word;
        int n_eff_word = 0;
        while (getline(ss, curr_word, ','))
        {
            curr_line_words.push_back(curr_word);
            int len = curr_word.length();
            int efflen = len;
            for(int i = 0; i < len; i++)
            {
                char ch = curr_word[i];
                if ((ch == '\357') || (ch == '\273') || (ch == '\277'))
                    efflen--;
                //cout << ch <<endl;
            }
            if(efflen > 1)n_eff_word++;
        }
        //strListLines.push_back(curr_line_words);
        if(n_eff_word > 0)
        {
            strListLines.push_back(curr_line_words);
            nArrEffWords.push_back(n_eff_word);
        }
    }
    // close file
    read_file.close();
}

// Columen Header
void Get_Column_Header(vector<string> &strColHeader)
{
    // Col 1 - Date, Col 2 - Transaction Description, Col 3 - Debit
    // Col 4 - Credit, Col 5- Currency, Col 6 - Card Name, Col 7 - Transaction, Col 8 - Location
    strColHeader.push_back("Date"); // Index -- 0
    strColHeader.push_back("Transaction Description");// Index -- 1
    strColHeader.push_back("Debit");// Index -- 2
    strColHeader.push_back("Credit");// Index -- 3
    strColHeader.push_back("Currency");// Index -- 4
    strColHeader.push_back("Card Name");// Index -- 5
    strColHeader.push_back("Transaction");// Index -- 6
    strColHeader.push_back("Location");// Index -- 7.. Total 8
}

void GetWordFromStringFromWordNo(string & strToAnalyze, int wordnofromlast, string &retstr)
{
    vector<string> arr_Words;
    
    string curr_str;
    int len = strToAnalyze.length();
    for(int i = 0; i < len; i++)
    {
        if ((strToAnalyze[i] == ' ') && (curr_str.length() > 0))
        {
            arr_Words.push_back(curr_str);
            curr_str = "";
        }
        else if(strToAnalyze[i] != ' ')
            curr_str += strToAnalyze[i];
    }
    if(curr_str.length()>0)arr_Words.push_back(curr_str);
    
    int nSzW = arr_Words.size();
    if ((nSzW-wordnofromlast) < 0)return;
    retstr = arr_Words[nSzW - wordnofromlast];
    
    if(2 == wordnofromlast)
    {
        strToAnalyze = "";
        for(int j = 0; j <= nSzW-2; j++)
        strToAnalyze += arr_Words[j] + " ";
    }
    
}

// GetCreditAndDebitFromAmountModel
void GetCreditAndDebitFromAmountModel(const string & strAmount, string &strCredit, string &strDebit)
{
    strCredit = "";
    strDebit = "";
    int len = strAmount.length();
    string tempstr;
    for(int i = 0; i < len; i++)
    {
        if ((strAmount[i] == 'c') || (strAmount[i] == 'C'))
        {
            strCredit = tempstr;
            //strCredit = stod(strCredit);
            strDebit = "0";
            //strDebit = stod(strDebit);
            break;
        }
        else if (strAmount[i] != '\r')
            tempstr += strAmount[i];
    }
    if(strCredit.empty())
    {
        strCredit = "0";
        //strCredit = stod(strCredit);
        strDebit = tempstr;
        //strDebit = stod(strDebit);
    }
}

// Remove New Line or Carraiage form string
void Remove_CarriageAndNewLineCharacter(string &strToRemoveChar)
{
    strToRemoveChar.erase(std::remove(strToRemoveChar.begin(), strToRemoveChar.end(), '\n'), strToRemoveChar.end());
    strToRemoveChar.erase(std::remove(strToRemoveChar.begin(), strToRemoveChar.end(), '\r'), strToRemoveChar.end());
}

// Convert to date time format
void ConvertToDateTimeFormat(string &strDateToDateTime)
{
//    time_t ttime = time(0);
//    //cout << ttime<< endl;
//    tm *local_time = localtime(&ttime);
    vector<int>ddmmyy;
    stringstream ss_curr(strDateToDateTime);
    string str_curr;
    while(getline(ss_curr,str_curr,'-'))
        ddmmyy.push_back(stoi(str_curr));

//    local_time->tm_hour = 00;
//    local_time->tm_min = 00;
//    local_time->tm_sec = 00;
    
    
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    timeinfo->tm_year = ddmmyy[2];
    timeinfo->tm_mon = ddmmyy[1];
    timeinfo->tm_mday = ddmmyy[0];
    timeinfo->tm_hour = 00;
    timeinfo->tm_min = 00;
    timeinfo->tm_sec = 00;
    //time (&rawtime);
    //timeinfo = localtime (&rawtime);

    strftime (buffer,80,"%x %H:%M:%S",timeinfo);
    //strftime (buffer,80,"%x %I:%M:%S%p",timeinfo);
    //strftime (buffer,80,"%a %b %e %H:%M:%S",timeinfo);
    
    strDateToDateTime =  buffer;
    
    //strDateToDateTime = strDateToDateTime + " " + buffer;
    //strDateToDateTime += " "+to_string(local_time->tm_hour)+":" + to_string(local_time->tm_hour)+":" + to_string(local_time->tm_sec);
}

// Convert to Double
void ConvertToDouble(string &str)
{
    int len = str.length();
    int nefflen = len;
    for(int i = 0; i < len; i++)
    {
        char ch = str[i];
        if ((ch == '\357') || (ch == '\273') || (ch == '\277') || (ch == 10) || (ch == 13) || (ch == 32))
            nefflen--;
    }
    
    if (nefflen == 0) str = "0";
    double tempnumd = stod(str);
    int tempnumi = stoi(str);
    if (tempnumd <= tempnumi)
        str += ".00";
}
// Covert to Desired format
void ConvertToDesiredFormat(vector<vector<string>> &strListLines, vector<vector<string>> &strConvertedLines,vector<int> & nEffWordsInLine)
{
    // Add colums for converted lines
   
    vector<string> col_header;
    Get_Column_Header(col_header);
    strConvertedLines.push_back(col_header);
    
    string strDate = ""; // Index 0
    string strTransDescr = ""; // Index 1
    string strDebit = ""; // Index 2
    string strCredit = ""; // Index 3
    string strCurrency = ""; // Index 4o
    string strCardName = ""; // Index 5
    string strTransaction = ""; // Index 6
    string strLocation = ""; // // Index 7
    int ndateidx = 0, nTransDesidx = 0 , nCridx = 0, nDebidx = 0, nAmtidx = 0;
    bool bAmtModel = false;
    bool bDomestic = false;
    
    int nTotalLines = strListLines.size();
    for(int nCurr_line = 0; nCurr_line < nTotalLines; nCurr_line++)
    {
        int nWords_In_Curr_line = strListLines[nCurr_line].size();
        int n_effWord = nEffWordsInLine[nCurr_line];
        bool appendline = true;
        for(int nWord_No = 0; nWord_No < nWords_In_Curr_line; nWord_No++)
        {
            string strword = strListLines[nCurr_line][nWord_No];
            
            if (strword.find("Domestic Transactions") != string::npos)
            {
                strTransaction = "Domestic";
                bDomestic = true;
                appendline = false;
                break;
            }
            else if ((strword.find("International Transactions") != string::npos) ||
                     (strword.find("International Transaction") != string::npos))
            {
                strTransaction = "International";
                bDomestic = false;
                appendline = false;
                break;
            }
            else if(strword.find("Date") != string::npos)
            {
                ndateidx = nWord_No;
                appendline = false;
                strCardName = "";
                continue;
            }
            else if((strword.find("Transaction Description") != string::npos) ||
                    (strword.find("Transaction Details") != string::npos))
            {
                nTransDesidx = nWord_No;
                appendline = false;
                continue;
            }
            else if(strword.find("Amount") != string::npos)
            {
                nAmtidx = nWord_No;
                bAmtModel = true;
                appendline = false;
                continue;
            }
            else if(strword.find("Credit") != string::npos)
            {
                nCridx = nWord_No;
                bAmtModel = false;
                appendline = false;
                continue;
            }
            else if(strword.find("Debit") != string::npos)
            {
                nDebidx = nWord_No;
                bAmtModel = false;
                appendline = false;
                continue;
            }
            else if (1 == n_effWord)
            {

                int len = strword.length();
                int nefflen = len;
                for(int i = 0; i < len; i++)
                {
                    char ch = strword[i];
                    if ((ch == '\357') || (ch == '\273') || (ch == '\277') || (ch == 10) || (ch == 13) || (ch == 32))
                        nefflen--;
                }
                if (nefflen > 0)strCardName = strword;
                appendline = false;
                continue;
            }

            strDate = strListLines[nCurr_line][ndateidx];
            strTransDescr = strListLines[nCurr_line][nTransDesidx];
            // Debit Credit Storage model
            if(bAmtModel)
                GetCreditAndDebitFromAmountModel(strListLines[nCurr_line][nAmtidx],strCredit,strDebit);
            else
            {
                strCredit = strListLines[nCurr_line][nCridx];
                strDebit = strListLines[nCurr_line][nDebidx];
            }
            if(strCredit.empty())strCredit = "0";
            if(strDebit.empty())strDebit = "0";
            //ConvertToDouble(strCredit);
            //ConvertToDouble(strDebit);
            // Currency
            if(bDomestic)
            {
                strCurrency = "INR";
                GetWordFromStringFromWordNo(strTransDescr,1,strLocation);
            }
            else
            {
                GetWordFromStringFromWordNo(strTransDescr,1,strCurrency);
                GetWordFromStringFromWordNo(strTransDescr,2,strLocation);
            }
            break;
            
        }
        
        if(appendline)
        {
            vector<string> curr_row;
            Remove_CarriageAndNewLineCharacter(strDate);
            //ConvertToDateTimeFormat(strDate);
            curr_row.push_back(strDate);
            Remove_CarriageAndNewLineCharacter(strTransDescr);
            curr_row.push_back(strTransDescr);
            Remove_CarriageAndNewLineCharacter(strDebit);
            curr_row.push_back(strDebit);
            Remove_CarriageAndNewLineCharacter(strCredit);
            curr_row.push_back(strCredit);
            Remove_CarriageAndNewLineCharacter(strCurrency);
            curr_row.push_back(strCurrency);
            Remove_CarriageAndNewLineCharacter(strCardName);
            curr_row.push_back(strCardName);
            Remove_CarriageAndNewLineCharacter(strTransaction);
            curr_row.push_back(strTransaction);
            Remove_CarriageAndNewLineCharacter(strLocation);
            curr_row.push_back(strLocation);
            strConvertedLines.push_back(curr_row);
        }
    }
}

// Write File
void Write_File(const string & outputFile, vector<vector<string>> &strConvertedLines)
{
    // Append lines in file after conversion to desired format
    fstream output_file;
    //output_file.open(outputFile, ios::out | ios::app);
    output_file.open(outputFile, ios::out);
    int len = strConvertedLines.size();
    for(int i = 0; i < len; i++)
    {
        int csize = strConvertedLines[i].size();
        for(int j = 0; j < csize; j++)
        {
            if(i > 0)
            {
                if ((j == 2) || (j == 3))
                    ConvertToDouble(strConvertedLines[i][j]);
                if(j == 0)
                    ConvertToDateTimeFormat(strConvertedLines[i][j]);
            }
            
            output_file << strConvertedLines[i][j];
            if(j != csize-1)output_file << ",";
        }
        output_file << "\n";
    }
    output_file.close();//close file
}

bool CompareDate(string &str1, string &str2)
{
    //bool result = false;;
    stringstream ss1(str1);
    stringstream ss2(str2);
    string ss1_word;
    string ss2_word;
    vector<int> date1;
    vector<int> date2;
    while(getline(ss1,ss1_word,'-'))
        date1.push_back(stoi(ss1_word));
    
    while(getline(ss2,ss2_word,'-'))
        date2.push_back(stoi(ss2_word));
    
    if (date1[2] < date2[2])
        return true;
    if ((date1[2] == date2[2]) && (date1[1] < date2[1]))
        return true;
    if ((date1[2] == date2[2]) && (date1[1] == date2[1])  && (date1[0] < date2[0]))
        return true;
    
    return false;
}
// Sorting On basis of dates
void SortOnBasisOfDates(vector<vector<string>> & strToSortOnBasisOfDates)
{
    // strToSortOnBasisOfDates[i][0]
    // First filter on year basis
    // Then filter on basis of month
    // on basis of data,
    // if time is included, filte on basis of time
    vector<string> strtemp;
    int nSize = strToSortOnBasisOfDates.size();
    for(int i = 1; i < nSize; i++)
        strtemp.push_back(strToSortOnBasisOfDates[i][0] + "-" + to_string(i));
    
    sort(strtemp.begin(), strtemp.end(),CompareDate);
    
    // Append new index in a array
    vector<int> arrpos;
    for(int j = 0; j <  nSize-1; j++)
    {
        // Get last no from string and append in array
        stringstream ss_curr(strtemp[j]);
        string str_curr;
        //string tempstr;
        while(getline(ss_curr,str_curr,'-'));
            //tempstr = str_curr;
        
        arrpos.push_back(stoi(str_curr));
    }
    
    vector<vector<string>> strtemptosort(nSize);
    strtemptosort[0] = strToSortOnBasisOfDates[0];
    
    for(int k = 1; k < nSize; k++)
    {
        strtemptosort[k] = strToSortOnBasisOfDates[arrpos[k-1]];
    }
    strToSortOnBasisOfDates = strtemptosort;
}
// StandardizeStatement -----------.
void StandardizeStatement(const string &inputFile, string &outputFile)
{
    //Check File Format
    if(!IsValidFileFormat(inputFile))
    {
        cout << "Invalid File extension, Give file path with valid Extension"<<endl;
        return;
    }
    
    // Create OutPut File Name
    GetOutPutFileName(inputFile,outputFile);
    if(outputFile.length() <= 0)
    {
        cout<<"Error in Creating File Name"<<endl;
        return;
    }
    
    // Append and Store data from file
    vector<vector<string>>  strListLines;
    vector<int> nEffWordsInLine;
    Read_File(inputFile,strListLines,nEffWordsInLine);
    // Convert to desired format
    vector<vector<string>> strConvertedStrings;
    ConvertToDesiredFormat(strListLines,strConvertedStrings,nEffWordsInLine);
    
    // Sort Converted String on basis of date
    SortOnBasisOfDates(strConvertedStrings);
    // Write Files
    Write_File(outputFile,strConvertedStrings);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    string inputFile = "";
    string outputFile = "";
    cin >> inputFile;
    StandardizeStatement(outputFile,outputFile);
    cout << endl;
    cout<<outputFile<<endl;
    return 0;
}


// TODO: Convert Date to DateTime Format ?
// TODO: Convert Amount to Double
// TODO: Sorting on basis of Date
