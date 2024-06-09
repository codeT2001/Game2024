#include "inc/listitembase.h"


ListItemBase::ListItemBase(QWidget *parent):QWidget(parent)
{

}

void ListItemBase::setItemType(ListItemType itemType)
{
    m_itemType = itemType;
}

ListItemType ListItemBase::getItemType()
{
    return m_itemType;
}
