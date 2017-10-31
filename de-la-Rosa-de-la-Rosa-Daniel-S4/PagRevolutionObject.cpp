#include "PagRevolutionObject.h"

PagRevolutionObject::PagRevolutionObject(std::vector<glm::vec2> points, unsigned int subdivisions,
	unsigned int slices) : sp(points), subdivisions(subdivisions), slices(slices)
{

	if (subdivisions > 0) {
		this->sp = sp.subdivide(subdivisions);
	}

	//Tomamos los puntos finales del perfil de subdvisi�n.
	auto profile_pts = this->sp.getPoints();

	glm::vec2 pt;
	//Para la coordenada 'v' de textura.
	float mod_accum = 0;
	float mod_total = 0;

	//Primer procesamiento para calcular la suma de los m�dulos de los 
	//vectores entre el primer punto y el �ltimo.
	for (int i = 0; i < profile_pts.size() - 1; i++) {
		pt = profile_pts[i];
		auto nextpt = profile_pts[i+1];
		auto vi = nextpt - pt;
		mod_total += sqrt((pow(vi.x, 2)) + pow(vi.y, 2));
	}

	float delta = 360.0f / slices;
	float a;
	PagPosNorm ppn;

	//PRIMER PUNTO.
	pt = profile_pts[0];

	//Si tiene tapa abajo, el primer punto que a�adimos es ese, con normal (0, -1, 0),
	//tangente (-1, 0, 0) y coord. de textura (0.5, 0.5). Lo hacemos directamente.
	if (sp.hasBottomFan()) {
		ppn.position = glm::vec3(0, pt.y, 0);
		ppn.normal = glm::vec3(0, -1, 0);

		pos_norm_bottom_fan.push_back(ppn);
		tangents_bottom_fan.push_back(glm::vec3(-1, 0, 0));
		texcoord_bottom_fan.push_back(glm::vec2(0.5, 0.5));
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

			//Calculamos el �ngulo a.
			a = s * delta;
			//Calculamos el punto haciendo la revoluci�n y su normal de la misma forma.
			//Adem�s, calculamos la tangente.
			glm::vec3 p(pt.x * cos(a * glm::pi<float>() / 180.0f), pt.y, -pt.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 np(v.x * cos(a * glm::pi<float>() / 180.0f), v.y, -v.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 tg(-sin(a * glm::pi<float>() / 180.0f), 0, -cos(a * glm::pi<float>() / 180.0f));

			//Calculamos la coordenada de textura.
			float text_u = s / (float)slices;

			//Almacenamos. 
			ppn.position = p;
			ppn.normal = np;

			pos_norm_body.push_back(ppn);
			tangents_body.push_back(tg);
			texcoord_body.push_back(glm::vec2(text_u, 0.0f));
		}
	}


	//CASO GENERAL. (Cuerpo).
	//Desde el segundo punto hasta el pen�ltimo.
	//Para cada punto, para cada corte longitudinal...
	for (int i = 1; i < profile_pts.size() - 1; i++) {
		//Obtenemos el punto y calculamos la normal a partir de sus dos vecinos.
		pt = profile_pts[i];
		glm::vec2 top_pt = profile_pts.at(i + 1);
		glm::vec2 bottom_pt = profile_pts.at(i - 1);

		//Calculamos el vector que va desde el punto hasta su vecino superior
		//y desde el vecino inferior hasta el punto.
		glm::vec2 v1 = pt - bottom_pt;
		glm::vec2 v2 = top_pt - pt;

		//Calculamos coord. 'v' de textura.
		//Calculamos modulo y lo sumamos a mod_accum.
		mod_accum += sqrt((pow(v1.x, 2)) + pow(v1.y, 2));
		//Dividimos mod_accum entre mod_total.
		float text_v = mod_accum / mod_total;

		//Calculamos normal
		v1 = glm::vec2(v1.y, -v1.x);
		v2 = glm::vec2(v2.y, -v2.x);
		v1 = v1 / (sqrt(v1.x*v1.x + v1.y*v1.y));
		v2 = v2 / (sqrt(v2.x*v2.x + v2.y*v2.y));
		//Aplicamos la f�rmula para hallar la normal.
		glm::vec2 vn = (v1 + v2) / 2.0f;
		//Normalizamos.
		vn = vn / (sqrt(vn.x*vn.x + vn.y*vn.y));

		for (int s = 0; s <= slices; s++) {
			//Calculamos el �ngulo a.
			a = s * delta;
			//Calculamos el punto haciendo la revoluci�n y su normal de la misma forma.
			//Adem�s, calculamos la tangente.
			glm::vec3 p(pt.x * cos(a * glm::pi<float>() / 180.0f), pt.y, -pt.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 np(vn.x * cos(a * glm::pi<float>() / 180.0f), vn.y, -vn.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 tg(-sin(a * glm::pi<float>() / 180.0f), 0, -cos(a * glm::pi<float>() / 180.0f));
			glm::vec2 tc(s / (float)slices, text_v);
			
			//Almacenamos
			ppn.position = p;
			ppn.normal = np;

			pos_norm_body.push_back(ppn);
			tangents_body.push_back(tg);
			texcoord_body.push_back(tc);

			//Si es el segundo punto y tiene tapa de abajo, este punto tambi�n pertenece a ella.
			if ((i == 1) && (sp.hasBottomFan())) {
				pos_norm_bottom_fan.push_back(ppn);
				tangents_bottom_fan.push_back(tg);
				//Calculamos su coordenada de textura de la tapa.
				glm::vec2 fan_tc((cos(a * glm::pi<float>() / 180.0f) + 1.0f) / 2.0f, (sin(a * glm::pi<float>() / 180.0f) + 1.0f) / 2.0f);
				texcoord_bottom_fan.push_back(fan_tc);
			}
			//Si es el pen�ltimo punto y tiene tapa de arriba, este punto tambi�n pertenece a ella.
			if ((i == profile_pts.size() - 2) && (sp.hasTopFan())){
				pos_norm_top_fan.push_back(ppn);
				tangents_top_fan.push_back(tg);
				//Calculamos su coordenada de textura de la tapa.
				glm::vec2 fan_tc((cos(a * glm::pi<float>() / 180.0f) + 1.0f) / 2.0f, (sin(a * glm::pi<float>() / 180.0f) + 1.0f) / 2.0f);
				texcoord_top_fan.push_back(fan_tc);
			}
		}
	}


	//PUNTO FINAL DEL PERFIL.
	//Si tiene tapa arriba, el punto que a�adimos es ese, con normal (0, 1, 0)
	//y tangente (0, 0, -1). Lo hacemos directamente.
	if (sp.hasTopFan()) {
		ppn.position = glm::vec3(0, pt.y, 0);
		ppn.normal = glm::vec3(0, 1, 0);

		pos_norm_top_fan.push_back(ppn);
		tangents_top_fan.push_back(glm::vec3(0, 0, -1));
		texcoord_top_fan.push_back(glm::vec2(0.5, 0.5));
	}
	else {
		//Aunque no tenga tapa de arriba, el �ltimo punto es un caso especial
		//ya que solo tiene un vecino para calcular la normal: el de abajo.
		pt = profile_pts[profile_pts.size() - 1];

		for (int s = 0; s <= slices; s++) {
			//Tomamos el vecino de abajo.
			glm::vec2 bottom_pt = profile_pts.at(profile_pts.size() - 2);
			//Calculamos el vector que va desde el vecino inferior hasta el punto.
			glm::vec2 v = pt - bottom_pt;
			//Hacemos el vector unitario.
			v = v / (sqrt(v.x*v.x + v.y*v.y));
			//Giramos 90 grados y obtenemos el vector normal.
			v = glm::vec2(v.y, -v.x);

			//Calculamos el �ngulo a.
			a = s * delta;
			//Calculamos el punto haciendo la revoluci�n y su normal de la misma forma.
			//Adem�s, calculamos la tangente.
			glm::vec3 p(pt.x * cos(a * glm::pi<float>() / 180.0f), pt.y, -pt.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 np(v.x * cos(a * glm::pi<float>() / 180.0f), v.y, -v.x * sin(a * glm::pi<float>() / 180.0f));
			glm::vec3 tg(-sin(a * glm::pi<float>() / 180.0f), 0, -cos(a * glm::pi<float>() / 180.0f));
			float text_u = s / (float)slices;

			//Almacenamos
			ppn.position = p;
			ppn.normal = np;

			pos_norm_body.push_back(ppn);
			tangents_body.push_back(tg);
			texcoord_body.push_back(glm::vec2(text_u, 1.0f));
		}
	}



	this->createTopology4PointCloud();

	std::cout << "createVBO: " << vaoBody.createVBOPosNorm() << std::endl;
	std::cout << "createIBO4PointCloud: " << vaoBody.createIBO4PointCloud() << std::endl;
	std::cout << "fillVBO: " << vaoBody.fillVBOPosNorm(pos_norm_body) << std::endl;
	std::cout << "fillIBO4PointCloud: " << vaoBody.fillIBO4PointCloud(i4PointCloud_body) << std::endl;

}


PagRevolutionObject::~PagRevolutionObject()
{
}

//Rellena los vectores de �ndices para dibujar como nube de puntos
void PagRevolutionObject::createTopology4PointCloud() {

	if (sp.hasBottomFan()) {
		//Se a�aden todos menos el �ltimo que es repetido.
		for (int i = 0; i < pos_norm_bottom_fan.size() - 1; i++) {
			i4PointCloud_bottomFan.push_back(i);
		}
	}

	if (sp.hasTopFan()) {
		//Se a�aden todos menos el primero que es repetido.
		for (int i = 1; i < pos_norm_top_fan.size(); i++) {
			i4PointCloud_topFan.push_back(i);
		}
	}

	if (sp.hasBody()) {
		//A�ade todos menos los que se repiten
		for (int i = 1; i < pos_norm_body.size(); i++) {
			if (!(i % (slices + 1) == 0)) {
				i4PointCloud_body.push_back(i);
			}
		}
	}
}


void PagRevolutionObject::createTopology4TriangleMesh() {

	auto puntosPerfil = sp.getPoints().size();

	if (sp.hasBottomFan()) {
		for (int i = 0; i < pos_norm_bottom_fan.size() - 1; i++) {
			i4PointCloud_bottomFan.push_back(i);
		}
	}

	if (sp.hasTopFan()) {
		for (int i = 1; i < pos_norm_top_fan.size(); i++) {
			i4PointCloud_topFan.push_back(i);
		}
	}

	if (sp.hasBody()) {
		for (int s = 0; s < this->slices; s++) {
			for (int i = 0; i < puntosPerfil; i++) {
				i4TriangleMesh_body.push_back((i * (this->slices + 1)) + s);
				i4TriangleMesh_body.push_back((i * (this->slices + 1)) + s + 1);
			}
			i4TriangleMesh_body.push_back(0xFFFF);
		}
	}

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
			std::cout <<i<<" -> " <<"Posici�n (" << pos_norm_body[i].position.x << ", " << pos_norm_body[i].position.y << ", " << pos_norm_body[i].position.z << "). ";
			std::cout << "Normal (" << pos_norm_body[i].normal.x << ", " << pos_norm_body[i].normal.y << ", " << pos_norm_body[i].normal.z << ")." << std::endl;
		}
		return pos_norm_body.data();
	}

	if ((part == PAG_TOP_FAN) && (sp.hasTopFan())) {
		for (int i = 0; i < pos_norm_top_fan.size(); i++) {
			std::cout << i << " -> " << "Posici�n (" << pos_norm_top_fan[i].position.x << ", " << pos_norm_top_fan[i].position.y << ", " << pos_norm_top_fan[i].position.z << "). ";
			std::cout << "Normal (" << pos_norm_top_fan[i].normal.x << ", " << pos_norm_top_fan[i].normal.y << ", " << pos_norm_top_fan[i].normal.z << ")." << std::endl;
		}
		return pos_norm_top_fan.data();
	}

	if ((part == PAG_BOTTOM_FAN) && (sp.hasBottomFan())) {
		for (int i = 0; i < pos_norm_bottom_fan.size(); i++) {
			std::cout << i << " -> " << "Posici�n (" << pos_norm_bottom_fan[i].position.x << ", " << pos_norm_bottom_fan[i].position.y << ", " << pos_norm_bottom_fan[i].position.z << "). ";
			std::cout << "Normal (" << pos_norm_bottom_fan[i].normal.x << ", " << pos_norm_bottom_fan[i].normal.y << ", " << pos_norm_bottom_fan[i].normal.z << ")." << std::endl;;
		}
		return pos_norm_bottom_fan.data();
	}

	return nullptr;
}

//Devuelve un array de C++ con las tangentes de los puntos correspondientes
//a la parte que se le pasa.
glm::vec3 * PagRevolutionObject::getTangents(PagRevObjParts part)
{

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

//Devuelve un array de C++ con las coordenadas de textura de cada v�rtice
//de la parte del objeto que se pasa como argumento. 
glm::vec2 * PagRevolutionObject::getTextureCoords(PagRevObjParts part)
{

	if ((part == PAG_BODY) && (sp.hasBody())) {
		for (int i = 0; i < texcoord_body.size(); i++) {
			std::cout << "Coord. textura: (" << texcoord_body[i].x << ", " << texcoord_body[i].y << ")" << std::endl;
		}
		return texcoord_body.data();
	}

	if ((part == PAG_TOP_FAN) && (sp.hasTopFan())) {
		for (int i = 0; i < texcoord_top_fan.size(); i++) {
			std::cout << "Coord. textura: (" << texcoord_top_fan[i].x << ", " << texcoord_top_fan[i].y << ")" << std::endl;
		}
		return texcoord_top_fan.data();
	}

	if ((part == PAG_BOTTOM_FAN) && (sp.hasBottomFan())) {
		for (int i = 0; i < texcoord_bottom_fan.size(); i++) {
			std::cout << "Coord. textura: (" << texcoord_bottom_fan[i].x << ", " << texcoord_bottom_fan[i].y << ")" << std::endl;
		}
		return texcoord_bottom_fan.data();
	}

	return nullptr;
}


GLuint* PagRevolutionObject::getIndices4PointCloud(PagRevObjParts part)
{

	if ((part == PAG_BODY) && (sp.hasBody())) {
		std::cout << "Indices4PointCloud cuerpo: " << std::endl;
		for (int i = 0; i < i4PointCloud_body.size(); i++) {
			std::cout << "- " << i4PointCloud_body[i] << " -" << std::endl;
		}
		
		return i4PointCloud_body.data();
	}

	if ((part == PAG_TOP_FAN) && (sp.hasTopFan())) {
		std::cout << "Indices4PointCloud tapa arriba: " << std::endl;
		for (int i = 0; i < i4PointCloud_topFan.size(); i++) {
			std::cout << "- " << i4PointCloud_topFan[i] << " -" << std::endl;
		}

		return i4PointCloud_topFan.data();
	}

	if ((part == PAG_BOTTOM_FAN) && (sp.hasBottomFan())) {
		std::cout << "Indices4PointCloud tapa abajo: " << std::endl;
		for (int i = 0; i < i4PointCloud_bottomFan.size(); i++) {
			std::cout << "- " << i4PointCloud_bottomFan[i] << " -" << std::endl;
		}

		return i4PointCloud_bottomFan.data();
	}

	return nullptr;
}


void PagRevolutionObject::drawAsPointCloud(PagRevObjParts part) {
	vaoBody.drawAsPointCloud();
}