#include <Keypad.h>
#include <LiquidCrystal.h>
#include <math.h>

//Создание экземпряла класса "жк-дисплей"
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

/*
	Команды
    C - очистка
    # - ввод для переменных a, b, f
    D - детали значений переменных
    A - обнуление переменных
*/

//Переменные для записи промежутка
double a = 0.0;
double b = 0.0;

//Результат
double result;

//Количество разбиений
int n = 1000;

//переменная выбора f
int select;

//переменные дял ввода значений
char customKey_select;
char customKey1;
char customKey2;

const byte ROWS = 4;
const byte COLS = 4;

//Установление значений клавиатуры
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {7,6,5,4};
byte colPins[COLS] = {3,2,1,0};


//Функции
double f_sin(double x)
{
   return sin(x);
}

double f_cos(double x)
{
   return cos(x);
}

double f_ln(double x)
{
   return log(x);
}

double f_exp(double x)
{
   return exp(x);
}

//Указатель на функцию
double(*select_f)(double);

//Функция для выбора математической функции
void select_func(int select)
{
  if(select >= 1 && select <=5)
  {
    switch(select)
    {
      case 1:
      {
        select_f = f_sin;
        break;
      }

      case 2:
      {
        select_f = f_cos;
        break;
      }

      case 3:
      {
        select_f = f_ln;
        break;
      }

      case 4:
      {
        select_f = f_exp;
        break;
      }
    }
  }
}

//функция вычисления методом трапеций
double trapezoid(float a, float b, int n){
  float h = (b - a) / n;
  double sum = 0.0;
  for(int i = 1; i < n; i++){
      double x = a + h*i;

      sum += select_f(x);
  }
  sum += (select_f(a) + select_f(b)) / 2.0;
  sum *= h;
  return sum;
};

//Функции для вывода информации
void info()
{
  lcd.clear();
  lcd.print("");
  lcd.print("1:sin,2:cos,3:ln");

  lcd.setCursor(0, 1);
  lcd.print("4:exp");
  delay(3000);
  lcd.clear();
}

//Функция показа значений переменных
void view_set_values()
{
  	//Очистить дисплей, поставить курсор в положение (0,0)
  	//и вывести переменные для установки положение
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("a=");
    lcd.setCursor(2, 0);
    lcd.print(double(a));

    lcd.setCursor(0, 1);
    lcd.print("b=");
    lcd.setCursor(2, 1);
    lcd.print(double(b));

    lcd.setCursor(6, 1);
    lcd.print("|f=");
    lcd.setCursor(9, 1);
    lcd.print(select); 
}

//Функция вывода результатов и обнуление
//переменных для следующего ввода
void results()
{
    lcd.clear();
    result = trapezoid(a, b, n);
    lcd.setCursor(0,0);
    lcd.print("result=");
    lcd.setCursor(7,0);
    lcd.print(result);
    delay(2000);
    lcd.clear();
    a = 0.0;
    b = 0.0;
    result = 0.0;
    select = 0;
    select_f = nullptr; //Нулевой указатель
    info();
}

//Функция обнуления переменных
void clean_var()
{
    a = 0.0;
    b = 0.0;
    result = 0.0;
    select = 0;
    select_f = nullptr; //Нулевой указатель
}


//объект класса Keypad
Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  lcd.begin(16, 2); 
  info();
}

void loop()
{
    // Выбрать функцию
    lcd.print("f=");
    while(1)
    {
      // Получение значений ввода
      customKey_select = customKeypad.getKey();
      if (customKey_select >='1' && customKey_select <='4')
      {
        select = select*10 + (customKey_select -'0');
       	select_func(int(select));
        lcd.setCursor(2,0);
        lcd.print(select);
        lcd.clear();
        break;
        
      }
      if(customKey_select == 'A')
      { 
          clean_var();
          lcd.clear();
          select_f = nullptr;
          break;
      }
      // Очистка
      if (customKey_select == 'C') 
      {
        lcd.clear();
        break;
      }
      // Вывети переменные и их значения
      if (customKey_select == 'D')
      {
        view_set_values();
      }
      if (customKey_select == '#')
      {
        lcd.clear();
        lcd.print("complete set f");
        lcd.setCursor(0, 1);
        lcd.print("f=");
        lcd.setCursor(2, 1);
        lcd.print(select);
      }
    }
  
    //Установка a
    lcd.print("a=");
    while(1)
    {
      customKey1 = customKeypad.getKey();
      if(customKey1 == 'C')
      { 
        lcd.clear();
        break;
      }
      
      if(customKey1 == 'A')
      { 
          clean_var();
          lcd.clear();
          select_f = nullptr;
          break;
      }
      
      if (customKey1 >='0' && customKey1 <='9') 
      {
        a = a*10 + (customKey1 -'0');
        lcd.setCursor(2,0);
        lcd.print(a);
      }
      
      if (customKey1 == 'D')
      {
        view_set_values();
      }
   
       if (customKey1 == '#')
       {
         lcd.clear();
         lcd.print("complete set a");
         lcd.setCursor(0, 1);
         lcd.print("a=");
         lcd.setCursor(2, 1);
         lcd.print(a);
       }
      
       if (customKey1 == '*') 
       {
           results();
       }
    }
  	
    //Установка b
    lcd.print("b=");
    while(1)
    {
      customKey2 = customKeypad.getKey();
      if(customKey2 == 'C')
      { 
          lcd.clear();
          break;
      }
      if(customKey1 == 'A')
      { 
          clean_var();
          lcd.clear();
          select_f = nullptr;
          break;
      }
      //Если введена цифра, то увеличивать ее значение дальше
      if (customKey2 >='0' && customKey2 <='9') 
      {
        b = b*10 + (customKey2 -'0');
        lcd.setCursor(2,0);
        lcd.print(b);
      }
      
      if (customKey2 == 'D') 
      {
        view_set_values();
      }
   
      if (customKey2 == '#') 
      {
        lcd.clear();
        lcd.print("complete set b");
        lcd.setCursor(0, 1);
        lcd.print("b=");
        lcd.setCursor(2, 1);
        lcd.print(b);
      }

      if (customKey2 == '*') 
      {
        results();
      }
    }
}