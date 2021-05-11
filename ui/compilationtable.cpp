#include "compilationtable.h"

CompilationTable::CompilationTable()
{

}

CompilationTable::CompilationTable(const QStringList &code, const std::vector<Parser::BinaryData> &binData)
{
    codeData = code.toVector();
    for (const Parser::BinaryData& data : binData){
        binaryData.insert(
                    data.index,
                    qMakePair(data.address, data.data)
        );
    }
}

int CompilationTable::rowCount(const QModelIndex &parent) const
{
    return codeData.size();
}

int CompilationTable::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant CompilationTable::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (role == Qt::DisplayRole){
        if (col == 0){
            return QString("%1").arg(row + 1, 4, 10, QLatin1Char(' '));
        }
        else if(col == 1){
            auto binData = binaryData[row];
            if (!binData.second.empty()) {
                return QString("0x%1").arg(binData.first, 8, 16, QLatin1Char('0')).toUpper();
            }
            else {
                return QVariant();
            }
        }
        else if (col == 2){
            auto binData = binaryData[row];
            if (!binData.second.empty()){
                QString dataStr;
                for (u8 num : binData.second){
                    dataStr.append(QString("%1 ").arg(num, 2, 16, QLatin1Char('0')));
                }
                return dataStr.toUpper();
            }
            else {
                return QVariant();
            }
        }
        else if (col == 3){
            return codeData[row];
        }
    }

    return QVariant();
}

QVariant CompilationTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("№");
        case 1:
            return QString("Address");
        case 2:
            return QString("Binary");
        case 3:
            return QString("Code");
        }
    }
    return QVariant();
}
