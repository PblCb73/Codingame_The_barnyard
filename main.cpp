#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

class Matrix
{
private:
	// сама матрица
	vector< vector<double> > _matrix;

	// временная матрица для расчета детепрминанта
	vector< vector<double> > _matrixTemp;

	// Что бы удобнее было! Что бы каждый раз не писать _matrix.size();
	int _mSize;

public:
	//Конструктор
	Matrix():_mSize( 0 ){}

    //Ну на всякий случай :)))
	virtual ~Matrix(){}

	//Конструктор кописрования
	Matrix( const Matrix &matrix )
	{
		_matrix = matrix._matrix;
		_mSize = matrix._mSize;
	}
	

	//Перегрузка опретора присвоения
	Matrix& operator = ( const Matrix &matrix )
	{
		_matrix = matrix._matrix;
		_mSize = matrix._mSize;
	}

	
	//Добавление строки к матрице
	void addRow( vector<double> row )
	{
		_matrix.push_back( row );
		_mSize++;
	}

    //Транспонирование матрицы
	void transposition()
	{
	    vector< vector<double> > matrix( _matrix );
	    for( int i( 0 ) ; i < _matrix.size() ; ++i )
	    {
	        for( int j ( 0 ) ; j < _matrix.size() ; ++j )
	        {
	            matrix[ i ][ j ] = _matrix[ j ][ i ];
	        }
	    }
	    _matrix = matrix;
	}

	//Сервисная функция для вывода на печать значения определителя
	double getDeterminant()
	{
		_matrixTemp = _matrix;
		calculateTriangleMatrix();
		double d( 1 );
		for( int i( 0 ); i < _mSize; ++i )
		{
			d *= _matrixTemp[ i ][ i ];
		}
		return d;
	}

	//Замена столбца в матрице
	void replaceColumn( int column, vector<double> vecotr )
	{
		for( int i( 0 ); i < vecotr.size(); ++i )
		{
			_matrix[ i ][ column ] = vecotr[ i ];
		}
	}

private:
	//Умножение строки
	void multiplication( vector<double> &vec, double v )
	{
		//Ради изучения сделаем через лямбду
		for_each( vec.begin(), vec.end(), [ v ]( double &val )
		{
			val = val * v;
		});
	}

	//Приведение к треугольному виду
	void calculateTriangleMatrix()
	{
		for ( int i( 0 ); i < _mSize - 1; ++i )
		{
			zeroingCol( i );
		}
	}

	//Обнуление столбца
	void zeroingCol( int &col )
	{
		//Вычисляем столбец i
		for ( int i( _mSize - 1 ) ; i > col ; --i )
		{
			//Если на главной диаганали 0, то найдем первую строку != 0
			if ( _matrixTemp[ col ][ col ] == 0 )
			{
				for ( int ii( col + 1 ); ii < _mSize; ++ii )
				{
					//Если элемент в столбце i != 0, то суммируем найденную строку с текукщей строкой
					if ( _matrixTemp[ ii ][ col ] != 0 )
					{
	                    transform( _matrixTemp[ ii ].begin(), _matrixTemp[ ii ].end(), _matrixTemp[ col ].begin(), _matrixTemp[ col ].begin(), plus<double>() );
						break;
					}
				}
			}
			vector<double> tVec( _matrixTemp[ col ] );

			//Умножаем i строку на отношение элемента в i строке к елементу на главной диагонали
			multiplication( tVec, _matrixTemp[ i ][ col ] / _matrixTemp[ col ][ col ] );

			//Вычитаем из i строки получившуюся строку
			transform( tVec.begin(), tVec.end(), _matrixTemp[ i ].begin(), _matrixTemp[ i ].begin(),std::minus<double>() );
		}
	}
};


map<string,int> partIndex;
map<string, vector<double> > animalParams;


void initParams()
{
    partIndex.insert( pair< string, int >( "Eyes", 0 ) );
    partIndex.insert( pair< string, int >( "Heads", 1 ) );
    partIndex.insert( pair< string, int >( "Legs", 2 ) );
    partIndex.insert( pair< string, int >( "Wings", 3 ) );
    partIndex.insert( pair< string, int >( "Horns", 4 ) );
    

    animalParams.insert( pair< string, vector<double> >( "Cows", {2,1,4,0,2} ) );
    animalParams.insert( pair< string, vector<double> >( "Pegasi", {2,1,4,2,0} ) );
    animalParams.insert( pair< string, vector<double> >( "Demons", {4,1,4,2,4} ) );
    animalParams.insert( pair< string, vector<double> >( "Chickens", {2,1,2,2,0} ) );
    animalParams.insert( pair< string, vector<double> >( "Rabbits", {2,1,4,0,0} ) );
}


vector<double> getParams( const vector<string> &params, const string &animalName )
{
    vector<double> aParams;
    for( auto name : params )
    {
        aParams.push_back( animalParams[ animalName ][ partIndex[ name ] ] );
    }
    return aParams;
}


int main()
{
    initParams();
    int N;
    cin >> N; cin.ignore();
    
    vector<string> animals;
    
    for ( int i( 0 ); i < N; ++i ) {
        string species;
        cin >> species; cin.ignore();
        animals.push_back( species );
    }
    
    vector<double> allElement;
    vector<string> parts;
    for ( int i( 0 ); i < N; ++i ) {
        string thing;
        int number;
        cin >> thing >> number; cin.ignore();
        parts.push_back( thing );
        allElement.push_back( number );
    }    
    
    //Инициализируем параметры
    initParams();

    Matrix originalMatrix;
    
    //Заполняем матрицу
    for( auto animal:animals )
    {
        originalMatrix.addRow( getParams( parts, animal ) );
    }
    
    //Транспонируем матрицу для дальнейших вычислений
    originalMatrix.transposition();

    //Получим детерминант оригинальной матрицы
    double mainDet( originalMatrix.getDeterminant() );
    for( int i( 0 ) ; i < animals.size() ; ++i )
    {
        //Создадим новую матрицу
        Matrix m( originalMatrix );
        
        //В этой матрице заменим i-тый столбец
        m.replaceColumn( i, allElement );
        
        //Найдем детерминант модифицированной матрицы и разделим на ранее полученный детерминант
        std::cout << animals[ i ] <<" "<< m.getDeterminant() / mainDet << endl;
    }
}