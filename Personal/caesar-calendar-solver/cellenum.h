#ifndef CELLENUM_H
#define CELLENUM_H

#include <map>
#include <string>

enum class CellID
{
    open,
    wall,
    Jan,
    Feb,
    Mar,
    Apr,
    May,
    Jun,
    Jul,
    Aug,
    Sep,
    Oct,
    Nov,
    Dec,
    Sun,
    Mon,
    Tue,
    Wed,
    Thu,
    Fri,
    Sat,
    one,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    eleven,
    twelve,
    thirteen,
    fourteen,
    fifteen,
    sixteen,
    seventeen,
    eighteen,
    nineteen,
    twenty,
    twentyone,
    twentytwo,
    twentythree,
    twentyfour,
    twentyfive,
    twentysix,
    twentyseven,
    twentyeight,
    twentynine,
    thirty,
    thirtyone,
    P1,
    P2,
    P3,
    P4,
    P5,
    P6,
    P7,
    P8,
    P9,
    P10,
    error
};

std::map<CellID, std::string> CellIDMap = {
    {CellID::open, "open"},
    {CellID::wall, "wall"},
    {CellID::Jan, "Jan"},
    {CellID::Feb, "Feb"},
    {CellID::Mar, "Mar"},
    {CellID::Apr, "Apr"},
    {CellID::May, "May"},
    {CellID::Jun, "Jun"},
    {CellID::Jul, "Jul"},
    {CellID::Aug, "Aug"},
    {CellID::Sep, "Sep"},
    {CellID::Oct, "Oct"},
    {CellID::Nov, "Nov"},
    {CellID::Dec, "Dec"},
    {CellID::Sun, "Sun"},
    {CellID::Mon, "Mon"},
    {CellID::Tue, "Tue"},
    {CellID::Wed, "Wed"},
    {CellID::Thu, "Thu"},
    {CellID::Fri, "Fri"},
    {CellID::Sat, "Sat"},
    {CellID::one, "1"},
    {CellID::two, "2"},
    {CellID::three, "3"},
    {CellID::four, "4"},
    {CellID::five, "5"},
    {CellID::six, "6"},
    {CellID::seven, "7"},
    {CellID::eight, "8"},
    {CellID::nine, "9"},
    {CellID::ten, "10"},
    {CellID::eleven, "11"},
    {CellID::twelve, "12"},
    {CellID::thirteen, "13"},
    {CellID::fourteen, "14"},
    {CellID::fifteen, "15"},
    {CellID::sixteen, "16"},
    {CellID::seventeen, "17"},
    {CellID::eighteen, "18"},
    {CellID::nineteen, "19"},
    {CellID::twenty, "20"},
    {CellID::twentyone, "21"},
    {CellID::twentytwo, "22"},
    {CellID::twentythree, "23"},
    {CellID::twentyfour, "24"},
    {CellID::twentyfive, "25"},
    {CellID::twentysix, "26"},
    {CellID::twentyseven, "27"},
    {CellID::twentyeight, "28"},
    {CellID::twentynine, "29"},
    {CellID::thirty, "30"},
    {CellID::thirtyone, "31"},
    {CellID::P1, "P1"},
    {CellID::P2, "P2"},
    {CellID::P3, "P3"},
    {CellID::P4, "P4"},
    {CellID::P5, "P5"},
    {CellID::P6, "P6"},
    {CellID::P7, "P7"},
    {CellID::P8, "P8"},
    {CellID::P9, "P9"},
    {CellID::P10, "P10"}};

//tostring
std::string to_string(CellID cellID)
{
    return CellIDMap[cellID];
}

//string to enum 
CellID stringToCellID(std::string cellID)
{
    for (const auto &pair : CellIDMap)
    {
        if (pair.second == cellID)
        {
            return pair.first;
        }
    }
    return CellID::error;
}

#endif // CELLENUM_H