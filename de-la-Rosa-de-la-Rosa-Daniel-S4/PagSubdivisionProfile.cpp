#include "PagSubdivisionProfile.h"



PagSubdivisionProfile::PagSubdivisionProfile(std::vector<glm::vec2> points)
{

	this->initialize();

	glm::vec2 p, superior;

	//Comenzamos buscando el ultimo punto valido. El superior, de mayor y.
	p = points[points.size() - 1];
	int j = points.size() - 2;

	//Mientras x no sea mayor o igual que cero y haya puntos por revisar, continuamos buscando.
	while (!(equalZeroE(p.x) || p.x > 0) && j >= 0) {
		p = points[j];
		j--;
	}

	//Aquí tenemos el punto superior. Puede que no hubiera ningún punto válido y
	//este sea el último punto procesado. Si este fuera el caso, las próximas instrucciones
	//no harán nada y se comprobará más tarde. (j >= 0)
	superior = p;

	if (equalZeroE(p.x)) {
		//Si el punto superior tiene x = 0, el perfil tiene tapa superior.
		this->topFan = true;
	}

	//Procesamos los puntos que quedan. Desde el primero (inferiores) hasta el superior.
	for (int i = 0; i <= j; i++) {
		p = points[i];
		if (equalZeroE(p.x)) {
			//Si la coordenada x es 0, comprobamos si es el primer punto de abajo.
			//En ese caso, el objeto tendria tapa inferior.
			//En caso contrario, no es un punto valido.
			if (validPoints.size() == 0) {
				this->bottomFan = true;
				validPoints.push_back(p);
			}
		}
		else if (p.x > 0) {
			//Si la coordenada x es mayor que 0, es valida.
			validPoints.push_back(p);
		}
	}

	//Si el punto superior era válido, lo añadimos.
	if (j >= 0)
		validPoints.push_back(superior);

	//Una vez procesados todos los puntos, si hay dos o más válidos, el perfil es válido.
	//Y si hay más de tres puntos, el perfil tendrá cuerpo.
	if (validPoints.size() >= 2) {
		this->valid = true;

		if (validPoints.size() > 3) {
			this->body = true;
		}
	}

}


PagSubdivisionProfile::~PagSubdivisionProfile()
{
}

PagSubdivisionProfile PagSubdivisionProfile::subdivide(unsigned int times)
{
	std::vector<glm::vec2> newProfilePoints;
	std::vector<glm::vec2> aux = validPoints;

	if ((validPoints.size() > 2) && (times > 0)) {

		for (int j = 0; j < times; j++) {

			//Segundo punto.
			auto p1 = aux[0];
			auto p2 = aux[1];
			auto p3 = aux[2];

			auto h1 = (p1 + p2) / 2.0f;
			auto h2 = (p2 + p3) / 2.0f;

			p2 = p2 / 2.0f + h1 / 4.0f + h2 / 4.0f;

			newProfilePoints.push_back(p1);
			newProfilePoints.push_back(h1);
			newProfilePoints.push_back(p2);
			newProfilePoints.push_back(h2);

			//Desde i=2, hasta tam - 1. Aumentando de 1 en 1. Los h-1 (h1) son repetidos, iguales que los h+1 (h2) anteriores.
			for (int i = 2; i < aux.size() - 1; i++) {

				//CASO GENERAL
				//Desde el punto 3 hasta el penúltimo.
				auto h1 = newProfilePoints[newProfilePoints.size() - 1];
				auto p1 = aux[i - 1];
				auto p2 = aux[i];
				auto p3 = aux[i + 1];

				auto h2 = (p2 + p3) / 2.0f;
				p2 = p2 / 2.0f + h1 / 4.0f + h2 / 4.0f;

				newProfilePoints.push_back(p2);
				newProfilePoints.push_back(h2);

			}

			newProfilePoints.push_back(aux[aux.size() - 1]);

			aux = newProfilePoints;
			newProfilePoints.clear();
		}

		PagSubdivisionProfile newSP(aux);

		return newSP;

	}

	return *this;
}

bool PagSubdivisionProfile::isValid()
{
	return this->valid;
}

bool PagSubdivisionProfile::hasTopFan()
{
	return this->topFan;
}

bool PagSubdivisionProfile::hasBottomFan()
{
	return this->bottomFan;
}

bool PagSubdivisionProfile::hasBody()
{
	return this->body;
}

std::vector<glm::vec2> PagSubdivisionProfile::getPoints()
{
	return this->validPoints;
}

//Inicializa variables.
void PagSubdivisionProfile::initialize() {
	this->body = false;
	this->bottomFan = false;
	this->topFan = false;
	this->valid = false;
}

//Compara un numero con cero teniendo un intervalo epsilon.
bool PagSubdivisionProfile::equalZeroE(float a)
{
	float ep = glm::epsilon<float>();

	return abs(a) < ep;

}
