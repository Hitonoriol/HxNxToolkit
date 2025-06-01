#pragma once
#include <QListWidget>
#include <QVariant>

namespace HxUtil::Qt {
	template<typename T>
	T getUserObject(QListWidgetItem* item)
	{
		auto var = item->data(::Qt::ItemDataRole::UserRole);
		auto obj = var.value<T>();
		return obj;
	}

	template<typename T>
	void setUserObject(QListWidgetItem* item, T&& obj)
	{
		QVariant var;
		var.setValue(obj);
		item->setData(::Qt::ItemDataRole::UserRole, var);
	}

	template<typename T>
	QVariant makeVariant(T&& obj)
	{
		QVariant var;
		var.setValue(obj);
		return var;
	}
}