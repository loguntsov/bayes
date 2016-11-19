Qualifier texts based on naive Bayesian classifier. 

Language of development: c ++ (gcc) 

Features: 

1. Education and classification with library libstemmer using as a stemmer. 
2. Education and classification using mystem program (http://company.yandex.ru/technologies/mystem/) with the ability to filter tokens using regular expressions. 
Only literal phrases converted, numbers, and punctuation marks don't convert to tokens and skipped. 

Example: 

a) Education: 
./bayes L 1.bayes 

Training sequence should be passed via stdin

For example (spam training \ not spam): 

----class 0 -----
give accountants services

----class 0 -----
rush to buy viagra

----class 1 -----
should buy milk


b) Classification 
./bayes C 1.bayes 

Classification sequence should be passed via stdin with 0 character at the end (to separate from the other classified text). 

c) For more information about the parameters: 
./bayes 

Example of work with PHP placed in ./php folder.

Program developed in IDE CodeBlocks.
Program works under UTF-8 coding, so ICU must be installed.
Tested in Ubuntu 12.04

-------------------------------------------------------------------------------

Классификатор текстов на основе наивного байесовского классификатора.

Язык разработки: c++ (gcc) 

Возможности:
1. Обучение и классификация c использованием библиотеки libstemmer в качестве стеммера.
2. Обучение и классификация с использованием программы mystem (http://company.yandex.ru/technologies/mystem/) с возможностью фильтрации токенов с помощью регулярного выражения.

Токенизируются только буквенные словосочетания, цифры и знаки препинания не токенизируются и пропускаются.

Пример использования:

а) Обучение:
./bayes L 1.bayes

на стандартный вход следует подать обучающую последовательность. Например (обучение спам\не спам):
----class 0 -----
предоставляю услуги бухгалтера 

----class 0 -----
спешите покупать виагру

----class 1 -----
купить надо молоко 

б) Классификация
./bayes C 1.bayes

на стандартный вход следует подать классифицируемый текст с 0 символом в конце (для отделения от другого классифицируемого текста). 

в) Более подробно про параметры:
./bayes

При использовании mystem данная программа должна находится рядом с bayes.


В каталоге ./php  пример работы с bayes из php.

IDE CodeBlocks.
Программа работает в кодировке UTF8, поэтому требуется установленная ICU.
Тестировалось в Ubuntu 12.04
