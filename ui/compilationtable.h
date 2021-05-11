#ifndef COMPILATIONTABLE_H
#define COMPILATIONTABLE_H

#include <QAbstractTableModel>
#include <QVector>

#include "../core/parser.h"
#include "../core/types.h"

class CompilationTable : public QAbstractTableModel
{
    Q_OBJECT
public:
    CompilationTable();
    CompilationTable(const QStringList& code, const std::vector<Parser::BinaryData>& binData);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
      QVector<QString> codeData;
      QMap<u32, QPair<u32, std::vector<u8>>> binaryData;
};

#endif // COMPILATIONTABLE_H
