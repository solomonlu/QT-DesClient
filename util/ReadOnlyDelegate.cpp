#include "ReadOnlyDelegate.h"

QWidget *ReadOnlyDelegate::createEditor(QWidget *,
                    const QStyleOptionViewItem &,
                    const QModelIndex &) const
{
    return NULL;
}

