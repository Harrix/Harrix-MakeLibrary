int HML_ClassOfTestFunction(TypeOfTestFunction Type)
{
    /*
    Функция выдает принадлежность тестовой функции к классу функций: бинарной, вещественной или иной оптимизации.
    Входные параметры:
     Type - тип тестовой функции.
     Смотреть виды в переменных перечисляемого типа в начале HarrixMathLibrary.h файла.
    Возвращаемое значение:
     Класс тестовой функции:
      1 - бинарной оптимизации;
      2 - вещественной оптимизации.
    */
    int VHML_Result=0;

    if (Type==TestFunction_SumVector)
        return 1;

    if (Type==TestFunction_Ackley)
        return 2;
		
	if (Type==TestFunction_AdditivePotential)
        return 2;
		
    if (Type==TestFunction_MultiplicativePotential)
        return 2;
		
	if (Type==TestFunction_ReverseGriewank)
        return 2;

    if (Type==TestFunction_ParaboloidOfRevolution)
        return 2;

    if (Type==TestFunction_Rastrigin)
        return 2;

    if (Type==TestFunction_Rosenbrock)
        return 2;
		
	if (Type==TestFunction_HyperEllipsoid)
        return 2;
		
	if (Type==TestFunction_RotatedHyperEllipsoid)
        return 2;
		
	if (Type==TestFunction_Wave)
        return 2;
		
	if (Type==TestFunction_Multiextremal)
        return 2;
		
	if (Type==TestFunction_Multiextremal2)
        return 2;
		
	if (Type==TestFunction_Sombrero)
        return 2;
		
	if (Type==TestFunction_Himmelblau)
        return 2;
		
	if (Type==TestFunction_Katnikov)
        return 2;
		
	if (Type==TestFunction_Multiextremal3)
        return 2;
		
	if (Type==TestFunction_Multiextremal4)
        return 2;
		
	if (Type==TestFunction_StepFunction)
        return 2;
		
	if (Type==TestFunction_Rana)
        return 2;
		
	if (Type==TestFunction_ShekelsFoxholes)
        return 2;
		
	if (Type==TestFunction_EggHolder)
        return 2;
		
	if (Type==TestFunction_RastriginWithChange)
        return 2;
		
	if (Type==TestFunction_RastriginWithTurning)
        return 2;
		
	if (Type==TestFunction_Schwefel)
        return 2;
		
	if (Type==TestFunction_Griewangk)
        return 2;
		
	if (Type==TestFunction_InvertedRosenbrock)
        return 2;
		
	if (Type==TestFunction_GaussianQuartic)
        return 2;
		
	if (Type==TestFunction_RastriginNovgorod)
        return 2;

    return VHML_Result;
}