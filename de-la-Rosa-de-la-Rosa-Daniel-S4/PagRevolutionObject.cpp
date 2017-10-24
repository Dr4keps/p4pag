#include "PagRevolutionObject.h"
#include <glm.hpp>
#include <vector>

PagRevolutionObject::PagRevolutionObject(std::vector<glm::vec2> points, unsigned int subdivisions,
	unsigned int slices) : sp(points), subdivisions(subdivisions), slices(slices)
{

	if (subdivisions > 0) {
		this->sp = sp.subdivide(subdivisions);
	}

	//Tomamos los puntos finales del perfil de subdvisión.
	auto profile_pts = this->sp.getPoints();

	//Tomamos el primer punto.
	glm::vec2 pt = profile_pts[0];

	float delta = 360.0f / slices;
	float a;

	//PRIMER PUNTO.
	//Si tiene tapa abajo, el primer punto que añadimos es ese, con normal (0, -1, 0)
	//y tangente (-1, 0, 0). Lo hacemos directamente.
	if (sp.hasBottomFan()) {
		PagPosNorm ppn;
		ppn.position = glm::vec3(0, pt.y, 0);
		ppn.normal = glm::vec3(0, -1, 0);

		pos_norm_bottom_fan.push_back(ppn);
		tangents_bottom_fan.push_back(glm::vec3(-1, 0, 0));
	}
	else {
		//Aunque no tenga tapa de abajo, el primer punto es un caso especial
		//ya que solo tiene un vecino para calcular la normal: el de arriba.

		for (int s = 0; s <= slices; s++) {
			//Calculamos la normal del punto.
			glm::vec2 top_pt = profile_pts.at(1);
			//Calculamos el vector que va desde el punto hasta su vecino superior.
			glm::vec2 v = top_pt - pt;
			//Lo hacemos unitario.
			v = v / (sqrt(v.x*v.x + v.y*v.y));
			//Giramos 90 grados y obtenemos el vector normal.
			v = glm::vec2(v.y, -v.x);

			//Calculamos el ángulo a.
			a = s * delta;
			//Calculamos el punto haciendo la revolución y su normal de la misma forma.
			//Además, calculamos la tangente.
			glm::vec3 p(pt.x * cos(a * glm::pi<float>() / 180.0f), pt.y, -pt.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 np(v.x * cos(a * glm::pi<float>() / 180.0f), v.y, -v.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 tg(-sin(a * glm::pi<float>() / 180.0f), 0, -cos(a * glm::pi<float>() / 180.0f));

			//Almacenamos
			PagPosNorm ppn;
			ppn.position = p;
			ppn.normal = np;

			pos_norm_body.push_back(ppn);
			tangents_body.push_back(tg);
		}
	}


	//CASO GENERAL. (Cuerpo).
	//Desde el segundo punto hasta el penúltimo. (El último también es especial).
	//Para cada punto, para cada corte longitudinal...
	for (int i = 1; i < profile_pts.size() - 1; i++) {
		//Obtenemos el punto y calculamos la normal a partir de sus dos vecinos.
		pt = profile_pts[i];
		glm::vec2 top_pt = profile_pts.at(i + 1);
		glm::vec2 bottom_pt = profile_pts.at(i - 1);
		//Calculamos el vector que va desde el punto hasta su vecino superior
		//y desde el vecino inferior hasta el punto y los giramos 90 grados.
		glm::vec2 v1 = pt - bottom_pt;
		glm::vec2 v2 = top_pt - pt;
		v1 = glm::vec2(v1.y, -v1.x);
		v2 = glm::vec2(v2.y, -v2.x);
		v1 = v1 / (sqrt(v1.x*v1.x + v1.y*v1.y));
		v2 = v2 / (sqrt(v2.x*v2.x + v2.y*v2.y));
		//Aplicamos la fórmula para hallar la normal.
		glm::vec2 v = (v1 + v2) / 2.0f;
		//Normalizamos.
		v = v / (sqrt(v.x*v.x + v.y*v.y));

		for (int s = 0; s <= slices; s++) {
			//Calculamos el ángulo a.
			a = s * delta;
			//Calculamos el punto haciendo la revolución y su normal de la misma forma.
			//Además, calculamos la tangente.
			glm::vec3 p(pt.x * cos(a * glm::pi<float>() / 180.0f), pt.y, -pt.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 np(v.x * cos(a * glm::pi<float>() / 180.0f), v.y, -v.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 tg(-sin(a * glm::pi<float>() / 180.0f), 0, -cos(a * glm::pi<float>() / 180.0f));

			//Almacenamos
			PagPosNorm ppn;
			ppn.position = p;
			ppn.normal = np;

			pos_norm_body.push_back(ppn);
			tangents_body.push_back(tg);

			//Si es el segundo punto y tiene tapa de abajo, este punto también pertenece a ella.
			if ((i == 1) && (sp.hasBottomFan())) {
				pos_norm_bottom_fan.push_back(ppn);
				tangents_bottom_fan.push_back(tg);
			}
			//Si es el penúltimo punto y tiene tapa de arriba, este punto también pertenece a ella.
			if ((i == profile_pts.size() - 2) && (sp.hasTopFan())){
				pos_norm_top_fan.push_back(ppn);
				tangents_top_fan.push_back(tg);
			}
		}
	}


	//PUNTO FINAL DEL PERFIL.
	//Si tiene tapa arriba, el punto que añadimos es ese, con normal (0, 1, 0)
	//y tangente (0, 0, -1). Lo hacemos directamente.
	if (sp.hasTopFan()) {
		PagPosNorm ppn;
		ppn.position = glm::vec3(0, pt.y, 0);
		ppn.normal = glm::vec3(0, 1, 0);

		pos_norm_top_fan.push_back(ppn);
		tangents_top_fan.push_back(glm::vec3(0, 0, -1));
	}
	else {
		//Aunque no tenga tapa de arriba, el último punto es un caso especial
		//ya que solo tiene un vecino para calcular la normal: el de abajo.
		pt = profile_pts[profile_pts.size() - 1];

		for (int s = 0; s <= slices; s++) {
			//Calculamos la normal del punto.
			//Tomamos el vecino de abajo.
			glm::vec2 bottom_pt = profile_pts.at(profile_pts.size() - 2);
			//Calculamos el vector que va desde el vecino inferior hasta el punto.
			glm::vec2 v = pt - bottom_pt;
			//Lo hacemos unitario.
			v = v / (sqrt(v.x*v.x + v.y*v.y));
			//Giramos 90 grados y obtenemos el vector normal.
			v = glm::vec2(v.y, -v.x);

			//Calculamos el ángulo a.
			a = s * delta;
			//Calculamos el punto haciendo la revolución y su normal de la misma forma.
			//Además, calculamos la tangente.
			glm::vec3 p(pt.x * cos(a * glm::pi<float>() / 180.0f), pt.y, -pt.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 np(v.x * cos(a * glm::pi<float>() / 180.0f), v.y, -v.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 tg(-sin(a * glm::pi<float>() / 180.0f), 0, -cos(a * glm::pi<float>() / 180.0f));

			//Almacenamos
			PagPosNorm ppn;
			ppn.position = p;
			ppn.normal = np;

			pos_norm_body.push_back(ppn);
			tangents_body.push_back(tg);
		}
	}


}


PagRevolutionObject::~PagRevolutionObject()
{
}

bool PagRevolutionObject::isValid()
{
	return this->sp.isValid();
}

bool PagRevolutionObject::has(PagRevObjParts part)
{
	if (part == PAG_BODY)
		return sp.hasBody();
	if (part == PAG_TOP_FAN)
		return sp.hasTopFan();
	if (part == PAG_BOTTOM_FAN)
		return sp.hasBottomFan();

	return false;
}

//Devuelve posiciones y normales de los puntos correspondiente a la PagRevObjParts
//que se le pasa como argumento.
PagPosNorm * PagRevolutionObject::getPositionsAndNormals(PagRevObjParts part)
{
	if ((part == PAG_BODY) && (sp.hasBody())) {
		for (int i = 0; i < pos_norm_body.size(); i++) {
			std::cout << "Posición (" << pos_norm_body[i].position.x << ", " << pos_norm_body[i].position.y << ", " << pos_norm_body[i].position.z << ")." << std::endl;
			std::cout << "Normal (" << pos_norm_body[i].normal.x << ", " << pos_norm_body[i].normal.y << ", " << pos_norm_body[i].normal.z << ")." << std::endl;
		}
		return pos_norm_body.data();
	}

	if ((part == PAG_TOP_FAN) && (sp.hasTopFan())) {
		return pos_norm_top_fan.data();
	}

	if ((part == PAG_BOTTOM_FAN) && (sp.hasBottomFan())) {
		return pos_norm_bottom_fan.data();
	}

	return nullptr;
}

glm::vec3 * PagRevolutionObject::getTangents(PagRevObjParts part)
{
	int index_begin = 0;
	int index_end = pos_norm_body.size() - 1;
	bool has_part = false;

	if ((part == PAG_BODY) && (sp.hasBody())) {
		return tangents_body.data();
	}

	if ((part == PAG_TOP_FAN) && (sp.hasTopFan())) {
		return tangents_top_fan.data();
	}

	if ((part == PAG_BOTTOM_FAN) && (sp.hasBottomFan())) {
		return tangents_bottom_fan.data();
	}

	return nullptr;
}
