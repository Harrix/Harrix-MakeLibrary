int HML_CountOfFitnessOfTestFunction_Binary(int Dimension)
{
    /*
    Функция определяет количество вычислений целевой функции для тестовых задач для единообразного сравнения алгоритмов.
    Обязательно вызвать один раз перед ее использованием функцию HML_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Для тестовых функций бинарного типа.
    Входные параметры:
     Dimension - размерность тестовой задачи. Может принимать значения: 20; 30; 40; 50; 60; 70; 80; 90; 100; 200.
    Возвращаемое значение:
     Количество вычислений целевой функции для тестовых задач.
    */
    int VHML_Result = 0;

    if (VHML_TypeOfTestFunction==TestFunction_SumVector)
    {
        if (Dimension==20)  VHML_Result = 225;
        if (Dimension==30)  VHML_Result = 400;
        if (Dimension==40)  VHML_Result = 576;
        if (Dimension==50)  VHML_Result = 784;
        if (Dimension==60)  VHML_Result = 1024;
        if (Dimension==70)  VHML_Result = 1296;
        if (Dimension==80)  VHML_Result = 1521;
        if (Dimension==90)  VHML_Result = 1764;
        if (Dimension==100) VHML_Result = 2025;
        if (Dimension==200) VHML_Result = 4761;
    }

    return VHML_Result;
}
//---------------------------------------------------------------------------
int HML_CountOfFitnessOfTestFunction_Binary(int Dimension, TypeOfTestFunction Type)
{
    /*
    Функция определяет количество вычислений целевой функции для тестовых задач для единообразного сравнения алгоритмов.
    Обязательно вызвать один раз перед ее использованием функцию HML_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Для тестовых функций бинарного типа.
    Входные параметры:
     Dimension - размерность тестовой задачи. Может принимать значения: 2; 3; 4; 5; 10; 20; 30;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Количество вычислений целевой функции для тестовых задач.
    */
    int VHML_Result = 0;

    VHML_TypeOfTestFunction = Type;

    VHML_Result = HML_CountOfFitnessOfTestFunction_Binary(Dimension);

    return VHML_Result;
}