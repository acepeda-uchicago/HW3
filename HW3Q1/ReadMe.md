# Question
In slide 30, we mentioned that overloads of operator+= can be either class member functions or global functions. 
Which one do you think is better and why?
# Answer
It depends on how you are using your += operator but generally I would lean towards using a class member for this type of overload.

In the example from the lecture we covered a class MyString which behaved like a string with some added features. With this example we may want to append a MyString with regular std::string variables and therefore a class member would be sufficient because the class method only works if the special class is on the left of the operand, which would be the case for how we would likely be using this MyString class i.e. appending regular strings to a MyString class would be more common than appending a MyString to a standard string class. 

In this example, I would not be able to use the global operator overload to overload a std::string += MyString anyway since std::string is a part of the C++ standard library and cannot be modified. 