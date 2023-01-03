# JsonParser

## Json的数据类型抽象

按照 http://www.json.org/json-en.html 中对Json的描述，Json数据类型由**object,array,string,bool,number,null**这六种基本数据类型构成，其中object和array又可以组成一定的嵌套关系，于是我们可以理解为Json的基本格式可以只有**string,bool,number,null**这四种构成。

在我的c++代码中，object类型由`std::unordered_map`来实现，array由`std::vector`来实现，即分别通过

`using object = std::unordered_map < std::string, Json >;`和`using array = std::vector<Json>;`来实现

## 代码的具体实现

代码总共由`Json`，`JsonValue`和`parser`这三个类构成。

### Json

首先我们将**object,array,string,bool,number,null**这六种基本数据类型都视为一个Json对象，即对于每一种数据类型，都使用class Json来储存。在class Json中，定义了一个`std::shared_ptr<JsonValue> m_ptr;`这样一个基类指针，并对每一种数据类型都有其各自的重载构造函数(定义指向不同派生类型的基类指针)。

### JsonValue

JsonValue是一个定义了`virtual void dump(std::string& out) const = 0;`这样一个纯虚函数的虚基类，并派生出`JsonNull`,`JsonDouble`,`JsonInt`,`JsonBoolean`,`JsonString`,`JsonArray`,`JsonObject`这几个子类，并分别重写其`dump`函数，最后通过在Json类中调用`m_ptr->dump();`来实现对不同数据类型的dump操作。

### parser

在parser类中对于用户输入进来的Json字符串格式进行解析并生成Json类。主要由`Json parse_json()`，`std::string parse_string()`和`Json parse_number()`三个函数构成。大体逻辑就是如果对于输入字符串str进行扫描，如果扫描到的字符为`'{'`则会生成一个哈希表，并对每一个键值对递归调用`Json parse_json()`，首先根据扫描到的字符串生成`键`，再直到扫描到的数据类型为最开始提到的四种基本类型，根据返回的基本类型，调用Json类相应的构造函数来生成`值`。如果扫描到`'['`，则相应生成vector并也执行上述递归操作。

### 异常

这部分并没有过多的实现，只是简单通过不同的情况来填写`msg`字符串，并通过`throw std::logic_error(msg);`来抛出相应的异常。

### 参考

https://github.com/dropbox/json11

http://www.json.org/json-en.html

## Example

```c++
    auto str = R"(
    {
      "name": "ZYB",
      "university": "杭电",
      "age": 22,
      "isMale": true,
      "PI":3.14,
      "Array":[1,true,"12\"34\t56"]
    }
  )";
    Json res = Json::parse(str);
    std::cout << res.dump() << std::endl;
```

