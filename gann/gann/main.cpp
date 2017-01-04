#include <iostream>
#include <time.h>
#include "graph.h"
#include "organism.h"
#include <string>
using namespace std;
float sigmoid(float x) { return 1 / (1 + exp(-x)); }
#define MAX_POP 20
#define INUM 5
#define HNUM 20
#define ONUM 2
organism* pops[MAX_POP],*newpops[MAX_POP];
#define X 155
#define Y 80
#define ROT (3.14/-4)
void initPopulation()
{
	for (int i = 0; i < MAX_POP; i++)
	{
		pops[i] = new organism(X,Y,ROT);
		pops[i]->nn = new ann(INUM, HNUM, ONUM, sigmoid);
	}
}
unsigned selectParentIndex(unsigned selectRank)
{
	int tot = pops[0]->getFitness();
	if (selectRank <= tot) { return 0; }
	int index = 0;
	for (int i = 0; i < MAX_POP - 1; i++)
	{
		if (selectRank > tot) { index = i + 1; }
		tot += pops[i]->getFitness();
	}
	return index;
}
#include <vector>
#include "vec2.h"
std::vector<vec2> blocks;
int main()
{
	int generation = 1;
	srand(time(0));
	graph::setHdc(GetDC(GetConsoleWindow()));
	graph::init();
	initPopulation(); // baþlangýç popülasyonu
	for (float i = 0; i < 4 * 3.14; i+=.12)
	{
		float ran = rand() % 18;
		organism::blocks.push_back({ 250 + cos(i) * (100+ran),200 + sin(i) * (100+ran) });
		organism::blocks.push_back({250+cos(i)*(180+ran),200+sin(i)*(180+ran)});
	}
	float speedup = 1;
	while (true)
	{
		speedup = min(100, speedup + (float)(GetAsyncKeyState(VK_RIGHT) ? 1 : 0)*.2f);
		speedup = max(1, speedup - (float)(GetAsyncKeyState(VK_LEFT) ? 1 : 0)*.2f);
		SetWindowText(GetConsoleWindow(), ("Generation: "+to_string(generation)).c_str());
		for (int i = 0; i < organism::blocks.size(); i++)
			graph::fillCircle(organism::blocks[i].x, organism::blocks[i].y, GLOBAL_RADIUS, RGB(0, 0, 0));
		bool herkesMefta = true;
		for (int i = 0; i < MAX_POP; i++)
		{
			if (!pops[i]->dead) { herkesMefta = false; }
			for (int k = 0; k < (int)speedup; k++)
			pops[i]->update();
		}
		if (herkesMefta) // doðal seleksiyon
		{
			generation++;
			int totalRank = 0;
			for (int i = 0; i < MAX_POP; i++) { totalRank += pops[i]->getFitness(); } // toplam derece
			int bestindex;
			int bestscore = -1;
			for (int i = 0; i < MAX_POP; i++)
			{
				int score = pops[i]->getFitness();
				if (bestscore < score) { bestscore = score; bestindex = i; }
			}
			organism *best = new organism(X, Y, ROT);
			best->nn = pops[bestindex]->nn;
			for (int i = 1; i < MAX_POP; i++)
			{
				int select = rand() % totalRank; // rastgele derece
				ann* mother = pops[selectParentIndex(select)]->nn; // anne seçimi
				select = rand() % totalRank; // diðer parent için
				ann* father = pops[selectParentIndex(select)]->nn; // baba seçimi
				organism *c = new organism(X, Y, ROT); // yeni bebek
				ann* baby = new ann(INUM, HNUM, ONUM, sigmoid); // yeni beyin
				for (int k = 0; k < INUM*HNUM; k++)
				{
					if (rand() & 1) { baby->i2h[k] = mother->i2h[k]; }
					else { baby->i2h[k] = father->i2h[k]; }
				}
				for (int k = 0; k < HNUM*ONUM; k++)
				{
					if (rand() & 1) { baby->h2o[k] = mother->h2o[k]; }
					else { baby->h2o[k] = father->h2o[k]; }
				}
				if (!(rand() % 2)) // mutasyon þansý
				{ 
					baby->i2h[rand() % (INUM*HNUM)] = ((float)rand() / RAND_MAX); 
					baby->h2o[rand() % (HNUM*ONUM)] = ((float)rand() / RAND_MAX);
				}
				c->nn = baby;
				newpops[i] = c;
			}
			for (int i = 0; i < MAX_POP; i++) { if (i != bestindex) { pops[i]->nn->~ann(); } }
			memcpy(pops, newpops, sizeof(pops));
			pops[0] = best;
		}
		graph::update();
	}
	return 0;
}