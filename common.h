#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <memory>

using namespace std;

template<class T>
class IocContainer
{
public:
	using FuncType = std::function<T* ()>;

	//注册一个key对应的类型
	template<class ClassType>
	void registerType(string key)
	{
		FuncType func = [] {return new ClassType(); };
		registerType(key, func);
	}

	//根据唯一标志查询对应的构造器 创建对象
	T* resolve(string key)
	{
		if (m_map.find(key) == m_map.end())
		{
			return nullptr;
		}
		auto func = m_map[key];
		return func();
	}
	std::shared_ptr<T> resolveShared(string key)
	{
		if (m_map.find(key) == m_map.end())
		{
			return nullptr;
		}
		auto func = m_map[key];
		return std::shared_ptr<T>(func());
	}

private:
	void registerType(string key, FuncType type)
	{
		if (m_map.find(key) != m_map.end())
		{
			throw std::invalid_argument("this key has exist");
		}
		m_map.emplace(key, type);
	}

private:
	std::map<string, FuncType> m_map;
};
