#include "art.h"
#include "utilities.h"
#include "status.h"

int Picture::getHeight()
{
	return size(); // the height of the picture is how many ColorStrings are in the vector
}

int Picture::getLength()
{
	int longestLine = 0;
	for (unsigned int i = 0; i < size(); i++)
	{
		int lineLength = at(i).getString().length();
		if (longestLine < lineLength)
		{
			longestLine = lineLength;
		}
	}
	return longestLine;
}


Picture Art::getTitle()
{
	Picture title; 
	int starColor = ddutil::COSMIC_COLOR;
	title.push_back(ColorString(R"(                     *             *            *             )", starColor));
	title.push_back(ColorString(R"(          *                 *             *         *   *     )", starColor));
	title.push_back(ColorString(R"(     *         *     *            *            *              )", starColor));
	title.push_back(ColorString(R"(        *)", starColor)+ColorString(R"(   _____  _   _ ______ _____ _   _ _____ )", TITLE_COLOR)+ColorString(R"(        *   )", starColor));
	title.push_back(ColorString(R"(    *     )", starColor)+ColorString(R"( /  __ \| | | || ___ \  _  | \ | |  _  | )", TITLE_COLOR)+ColorString(R"(  *        )", starColor));
	title.push_back(ColorString(R"(        *  )", starColor)+ColorString(R"(| /  \/| |_| || |_/ / | | |  \| | | | | )", TITLE_COLOR)+ColorString(R"(      *    )", starColor));
	title.push_back(ColorString(R"( *    *    )", starColor)+ColorString(R"(| |    |  _  ||    /| | | | . ` | | | |  )", TITLE_COLOR)+ColorString(R"(*       * )", starColor));
	title.push_back(ColorString(R"(           )", starColor)+ColorString(R"(| \__i\| | | || |\ \\ \_/ / |\  \ \_/ /  )", TITLE_COLOR)+ColorString(R"(   *      )", starColor));
	title.push_back(ColorString(R"(    *   *   )", starColor)+ColorString(R"(\____/\_| |_/\_| \_|\___/\_| \_/\___/  )", TITLE_COLOR)+ColorString(R"(*       *  )", starColor));
	title.push_back(ColorString(R"( * )", starColor)+ColorString(R"(______ ___________ _____ _   _______ ___________  _____    )", TITLE_COLOR));
	title.push_back(ColorString(R"(   )", starColor)+ColorString(R"(|  _  \  ___|  ___|  ___| \ | |  _  \  ___| ___ \/  ___| )", TITLE_COLOR)+ColorString(R"(* )", starColor));
	title.push_back(ColorString(R"(  *)", starColor)+ColorString(R"(| | | | |__ | |_  | |__ |  \| | | | | |__ | |_/ /\ `--.   )", TITLE_COLOR)+ColorString(R"( )", starColor));
	title.push_back(ColorString(R"(   )", starColor)+ColorString(R"(| | | |  __||  _| |  __|| . ` | | | |  __||    /  `--. \  )", TITLE_COLOR)+ColorString(R"(*)", starColor));
	title.push_back(ColorString(R"(*  )", starColor)+ColorString(R"(| |/ /| |___| |   | |___| |\  | |/ /| |___| |\ \ /\__/ /  )", TITLE_COLOR)+ColorString(R"( )", starColor));
	title.push_back(ColorString(R"(   )", starColor)+ColorString(R"(|___/ \____/\_|   \____/\_| \_/___/ \____/\_| \_|\____/  )", TITLE_COLOR)+ColorString(R"(* )", starColor));
	return title;
}

Picture Art::getPaladin()
{
	Picture paladin;
	paladin.push_back(ColorString(R"(               {}             )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(              .--.            )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(             /.--.\           )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(             |====|           )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(             |`::`|           )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(         .-;`\..../`;_.-^-._  )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  /\\   /  |...::..|`   :   `|)", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  |:'\ |   /'''::''|   .:.   |)", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"( @|\ /\;-,/\   ::  |..:::::..|)", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"( `||\ <` >  >._::_.| ':::::' |)", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  || `""`  /   ^^  |   ':'   |)", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  ||       |       \    :    /)", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  ||       |        \   :   / )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  ||       |___/\___|`-.:.-`  )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  ||        \_ || _/    `     )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  ||        <_ >< _>          )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  ||        |  ||  |          )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  ||        |  ||  |          )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  ||       _\.:||:./_         )", ddutil::PALADIN_COLOR));
	paladin.push_back(ColorString(R"(  \/      /____/\____\        )", ddutil::PALADIN_COLOR));
	return paladin;
}

Picture Art::getGunslinger(int color)
{
	Picture gunslinger;
	gunslinger.push_back(ColorString(R"(    \______          )", color));
	gunslinger.push_back(ColorString(R"(   __\o___|__        )", color));
	gunslinger.push_back(ColorString(R"(    ///  -.          )", color));
	gunslinger.push_back(ColorString(R"(    _/ \_c/          )", color));
	gunslinger.push_back(ColorString(R"(   /= \/             )", color));
	gunslinger.push_back(ColorString(R"(   \ ^|\     ______. )", color));
	gunslinger.push_back(ColorString(R"(   )\  \___~(_]---'  )", color));
	gunslinger.push_back(ColorString(R"(  _\_\,__o//_)       )", color));
	gunslinger.push_back(ColorString(R"( (x_\_:M             )", color));
	gunslinger.push_back(ColorString(R"(   \ *\ \            )", color));
	gunslinger.push_back(ColorString(R"(    \ *\ \           )", color));
	gunslinger.push_back(ColorString(R"(     \,/ `)          )", color));
	gunslinger.push_back(ColorString(R"(    / (   |          )", color));
	gunslinger.push_back(ColorString(R"(   ,- <___(          )", color));
	gunslinger.push_back(ColorString(R"(   (o \|  |          )", color));
	gunslinger.push_back(ColorString(R"(   [_,_(o \__        )", color));
	gunslinger.push_back(ColorString(R"(       \_,___]       )", color));
	return gunslinger;
}

Picture Art::getJesterGunslinger()
{
	Picture gunslinger;
	int color = JesterStatus::COLOR;
	gunslinger.push_back(ColorString(R"(    \______          )", color));
	gunslinger.push_back(ColorString(R"(   __\o___|__        )", color));
	gunslinger.push_back(ColorString(R"(    ///  -.          )", color));
	gunslinger.push_back(ColorString(R"(    _/ \_c/          )", color));
	gunslinger.push_back(ColorString(R"(   /= \/             )", color));
	gunslinger.push_back(ColorString(R"(   \ ^|\     ______. )", color));
	gunslinger.push_back(ColorString(R"(   )\  \___~(_]---'  )", color));
	gunslinger.push_back(ColorString(R"(  _\_\,__o//_)       )", color));
	gunslinger.push_back(ColorString(R"( (x_\_:M             )", color));
	gunslinger.push_back(ColorString(R"(   \ *\ \            )", color));
	gunslinger.push_back(ColorString(R"(    \ *\ \           )", color));
	gunslinger.push_back(ColorString(R"(     \,/ `)          )", color));
	gunslinger.push_back(ColorString(R"(    / (   |          )", color));
	gunslinger.push_back(ColorString(R"(   ,- <___(          )", color));
	gunslinger.push_back(ColorString(R"(   (o \|  |          )", color));
	gunslinger.push_back(ColorString(R"(   [_,_(o \__        )", color));
	gunslinger.push_back(ColorString(R"(       \_,___]       )", color));
	return gunslinger;
}

Picture Art::getCosmicGunslinger()
{
	Picture gunslinger;
	int color = ddutil::COSMIC_COLOR;
	int color2 = ddutil::LIGHTMAGENTA;
	gunslinger.push_back(ColorString(R"(    \______          )", color));
	gunslinger.push_back(ColorString(R"(   __\o___|__        )", color2));
	gunslinger.push_back(ColorString(R"(    ///  -.          )", color));
	gunslinger.push_back(ColorString(R"(    _/ \_c/          )", color2));
	gunslinger.push_back(ColorString(R"(   /= \/             )", color));
	gunslinger.push_back(ColorString(R"(   \ ^|\     ______. )", color2));
	gunslinger.push_back(ColorString(R"(   )\  \___~(_]---'  )", color));
	gunslinger.push_back(ColorString(R"(  _\_\,__o//_)       )", color2));
	gunslinger.push_back(ColorString(R"( (x_\_:M             )", color));
	gunslinger.push_back(ColorString(R"(   \ *\ \            )", color2));
	gunslinger.push_back(ColorString(R"(    \ *\ \           )", color));
	gunslinger.push_back(ColorString(R"(     \,/ `)          )", color2));
	gunslinger.push_back(ColorString(R"(    / (   |          )", color));
	gunslinger.push_back(ColorString(R"(   ,- <___(          )", color2));
	gunslinger.push_back(ColorString(R"(   (o \|  |          )", color));
	gunslinger.push_back(ColorString(R"(   [_,_(o \__        )", color2));
	gunslinger.push_back(ColorString(R"(       \_,___]       )", color));
	return gunslinger;
}

Picture Art::getWhiteBackgroundGunslinger()
{
	Picture gunslinger;
	int color = ddutil::getColor(ddutil::GUNSLINGER_COLOR, ddutil::WHITE);
	gunslinger.push_back(ColorString(R"(    \______          )", color));
	gunslinger.push_back(ColorString(R"(   __\o___|__        )", color));
	gunslinger.push_back(ColorString(R"(    ///  -.          )", color));
	gunslinger.push_back(ColorString(R"(    _/ \_c/          )", color));
	gunslinger.push_back(ColorString(R"(   /= \/             )", color));
	gunslinger.push_back(ColorString(R"(   \ ^|\     ______. )", color));
	gunslinger.push_back(ColorString(R"(   )\  \___~(_]---'  )", color));
	gunslinger.push_back(ColorString(R"(  _\_\,__o//_)       )", color));
	gunslinger.push_back(ColorString(R"( (x_\_:M             )", color));
	gunslinger.push_back(ColorString(R"(   \ *\ \            )", color));
	gunslinger.push_back(ColorString(R"(    \ *\ \           )", color));
	gunslinger.push_back(ColorString(R"(     \,/ `)          )", color));
	gunslinger.push_back(ColorString(R"(    / (   |          )", color));
	gunslinger.push_back(ColorString(R"(   ,- <___(          )", color));
	gunslinger.push_back(ColorString(R"(   (o \|  |          )", color));
	gunslinger.push_back(ColorString(R"(   [_,_(o \__        )", color));
	gunslinger.push_back(ColorString(R"(       \_,___]       )", color));
	return gunslinger;
}

Picture Art::getSorcerer(int color)
{
	Picture sorcerer;
	sorcerer.push_back(ColorString(R"(              \'/    )", color));
	sorcerer.push_back(ColorString(R"(            -= * =-  )", color));
	sorcerer.push_back(ColorString(R"(        .-"-. / #,_  )", color));
	sorcerer.push_back(ColorString(R"(       / /\_ \  `#|\ )", color));
	sorcerer.push_back(ColorString(R"(      / /')'\ \  /#/ )", color));
	sorcerer.push_back(ColorString(R"(     (  \ = /  )/\/# )", color));
	sorcerer.push_back(ColorString(R"(      )  ) (  (/  \  )", color));
	sorcerer.push_back(ColorString(R"(     (_.;`"`;._)  |  )", color));
	sorcerer.push_back(ColorString(R"(    / (  \|/  )   |  )", color));
	sorcerer.push_back(ColorString(R"(   /  /\-'^'-/\   |  )", color));
	sorcerer.push_back(ColorString(R"(  |  \| )=@=(  \_/   )", color));
	sorcerer.push_back(ColorString(R"(  |  /\/     \       )", color));
	sorcerer.push_back(ColorString(R"(  | /\ \      ;      )", color));
	sorcerer.push_back(ColorString(R"(  \(// /'     |      )", color));
	sorcerer.push_back(ColorString(R"(     \/       |      )", color));
	sorcerer.push_back(ColorString(R"(      |     / /      )", color));
	sorcerer.push_back(ColorString(R"(      | ___/\_\      )", color));
	sorcerer.push_back(ColorString(R"(      |/ / \ \|      )", color));
	sorcerer.push_back(ColorString(R"(     / | | | | \     )", color));
	sorcerer.push_back(ColorString(R"(     \_|/   \|_/     )", color));
	sorcerer.push_back(ColorString(R"(      / \`-'/ \      )", color));
	sorcerer.push_back(ColorString(R"(      `-'   '-`      )", color));
	return sorcerer;
}

Picture Art::getElementalSorceress()
{
	Picture sorcerer;
	int color = ElementalStatus::COLOR;
	sorcerer.push_back(ColorString(R"(              \'/    )", color));
	sorcerer.push_back(ColorString(R"(            -= * =-  )", color));
	sorcerer.push_back(ColorString(R"(        .-"-. / #,_  )", color));
	sorcerer.push_back(ColorString(R"(       / /\_ \  `#|\ )", color));
	sorcerer.push_back(ColorString(R"(      / /')'\ \  /#/ )", color));
	sorcerer.push_back(ColorString(R"(     (  \ = /  )/\/# )", color));
	sorcerer.push_back(ColorString(R"(      )  ) (  (/  \  )", color));
	sorcerer.push_back(ColorString(R"(     (_.;`"`;._)  |  )", color));
	sorcerer.push_back(ColorString(R"(    / (  \|/  )   |  )", color));
	sorcerer.push_back(ColorString(R"(   /  /\-'^'-/\   |  )", color));
	sorcerer.push_back(ColorString(R"(  |  \| )=@=(  \_/   )", color));
	sorcerer.push_back(ColorString(R"(  |  /\/     \       )", color));
	sorcerer.push_back(ColorString(R"(  | /\ \      ;      )", color));
	sorcerer.push_back(ColorString(R"(  \(// /'     |      )", color));
	sorcerer.push_back(ColorString(R"(     \/       |      )", color));
	sorcerer.push_back(ColorString(R"(      |     / /      )", color));
	sorcerer.push_back(ColorString(R"(      | ___/\_\      )", color));
	sorcerer.push_back(ColorString(R"(      |/ / \ \|      )", color));
	sorcerer.push_back(ColorString(R"(     / | | | | \     )", color));
	sorcerer.push_back(ColorString(R"(     \_|/   \|_/     )", color));
	sorcerer.push_back(ColorString(R"(      / \`-'/ \      )", color));
	sorcerer.push_back(ColorString(R"(      `-'   '-`      )", color));
	return sorcerer;
}

Picture Art::getCosmicSorceress()
{
	Picture sorcerer;
	int color = ddutil::COSMIC_COLOR;
	int color2 = ddutil::LIGHTMAGENTA;
	sorcerer.push_back(ColorString(R"(              \'/    )", color));
	sorcerer.push_back(ColorString(R"(            -= * =-  )", color2));
	sorcerer.push_back(ColorString(R"(        .-"-. / #,_  )", color));
	sorcerer.push_back(ColorString(R"(       / /\_ \  `#|\ )", color2));
	sorcerer.push_back(ColorString(R"(      / /')'\ \  /#/ )", color));
	sorcerer.push_back(ColorString(R"(     (  \ = /  )/\/# )", color2));
	sorcerer.push_back(ColorString(R"(      )  ) (  (/  \  )", color));
	sorcerer.push_back(ColorString(R"(     (_.;`"`;._)  |  )", color2));
	sorcerer.push_back(ColorString(R"(    / (  \|/  )   |  )", color));
	sorcerer.push_back(ColorString(R"(   /  /\-'^'-/\   |  )", color2));
	sorcerer.push_back(ColorString(R"(  |  \| )=@=(  \_/   )", color));
	sorcerer.push_back(ColorString(R"(  |  /\/     \       )", color2));
	sorcerer.push_back(ColorString(R"(  | /\ \      ;      )", color));
	sorcerer.push_back(ColorString(R"(  \(// /'     |      )", color2));
	sorcerer.push_back(ColorString(R"(     \/       |      )", color));
	sorcerer.push_back(ColorString(R"(      |     / /      )", color2));
	sorcerer.push_back(ColorString(R"(      | ___/\_\      )", color));
	sorcerer.push_back(ColorString(R"(      |/ / \ \|      )", color2));
	sorcerer.push_back(ColorString(R"(     / | | | | \     )", color));
	sorcerer.push_back(ColorString(R"(     \_|/   \|_/     )", color2));
	sorcerer.push_back(ColorString(R"(      / \`-'/ \      )", color));
	sorcerer.push_back(ColorString(R"(      `-'   '-`      )", color2));
	return sorcerer;
}

Picture Art::getWhiteBackgroundSorcerer()
{
	Picture sorcerer;
	int color = ddutil::getColor(ddutil::SORCERER_COLOR, ddutil::WHITE);
	sorcerer.push_back(ColorString(R"(              \'/    )", color));
	sorcerer.push_back(ColorString(R"(            -= * =-  )", color));
	sorcerer.push_back(ColorString(R"(        .-"-. / #,_  )", color));
	sorcerer.push_back(ColorString(R"(       / /\_ \  `#|\ )", color));
	sorcerer.push_back(ColorString(R"(      / /')'\ \  /#/ )", color));
	sorcerer.push_back(ColorString(R"(     (  \ = /  )/\/# )", color));
	sorcerer.push_back(ColorString(R"(      )  ) (  (/  \  )", color));
	sorcerer.push_back(ColorString(R"(     (_.;`"`;._)  |  )", color));
	sorcerer.push_back(ColorString(R"(    / (  \|/  )   |  )", color));
	sorcerer.push_back(ColorString(R"(   /  /\-'^'-/\   |  )", color));
	sorcerer.push_back(ColorString(R"(  |  \| )=@=(  \_/   )", color));
	sorcerer.push_back(ColorString(R"(  |  /\/     \       )", color));
	sorcerer.push_back(ColorString(R"(  | /\ \      ;      )", color));
	sorcerer.push_back(ColorString(R"(  \(// /'     |      )", color));
	sorcerer.push_back(ColorString(R"(     \/       |      )", color));
	sorcerer.push_back(ColorString(R"(      |     / /      )", color));
	sorcerer.push_back(ColorString(R"(      | ___/\_\      )", color));
	sorcerer.push_back(ColorString(R"(      |/ / \ \|      )", color));
	sorcerer.push_back(ColorString(R"(     / | | | | \     )", color));
	sorcerer.push_back(ColorString(R"(     \_|/   \|_/     )", color));
	sorcerer.push_back(ColorString(R"(      / \`-'/ \      )", color));
	sorcerer.push_back(ColorString(R"(      `-'   '-`      )", color));
	return sorcerer;
}

Picture Art::getMinotaur()
{
	Picture minotaur;
	minotaur.push_back(ColorString(R"(     .      .               )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(     |\____/|               )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(    (\|----|/)              )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(     \ 0  0 /               )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(      |    |                )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(   ___/\../\____            )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(  /     --       \          )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"( /  \         /   \         )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(|    \___/___/(   |         )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(\   /|  }{   | \  )         )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"( \  ||__}{__|  |  |         )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(  \  |;;;;;;;\  \ / \_______)", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(   \ /;;;;;;;;| [,,[|======')", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(     |;;;;;;/ |     /       )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(     ||;;|\   |             )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(     ||;;/|   /             )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(     \_|:||__|              )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(      \ ;||  /              )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(      |= || =|              )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(      |= /\ =|              )", ddutil::BROWN));
	minotaur.push_back(ColorString(R"(      /_/  \_\              )", ddutil::BROWN));
	return minotaur;
}

Picture Art::getFireImp()
{
	Picture fireImp;
	fireImp.push_back(ColorString(R"(           v   )", ddutil::RED));
	fireImp.push_back(ColorString(R"(     (__)v | v )", ddutil::RED));
	fireImp.push_back(ColorString(R"(     /\/\\_|_/ )", ddutil::RED));
	fireImp.push_back(ColorString(R"(    _\__/  |   )", ddutil::RED));
	fireImp.push_back(ColorString(R"(   /  \/`\<`)  )", ddutil::RED));
	fireImp.push_back(ColorString(R"(   \ (  |\_/   )", ddutil::RED));
	fireImp.push_back(ColorString(R"(   /)))-(  |   )", ddutil::RED));
	fireImp.push_back(ColorString(R"(  / /^ ^ \ |   )", ddutil::RED));
	fireImp.push_back(ColorString(R"( /  )^/\^( |   )", ddutil::RED));
	fireImp.push_back(ColorString(R"( )_//`__>> |   )", ddutil::RED));
	fireImp.push_back(ColorString(R"(   #   #`  |   )", ddutil::RED));
	return fireImp;
}

Picture Art::getIceImp()
{
	Picture iceImp;
	iceImp.push_back(ColorString(R"(           v   )", ddutil::LIGHTCYAN));
	iceImp.push_back(ColorString(R"(     (__)v | v )", ddutil::LIGHTCYAN));
	iceImp.push_back(ColorString(R"(     /\/\\_|_/ )", ddutil::LIGHTCYAN));
	iceImp.push_back(ColorString(R"(    _\__/  |   )", ddutil::LIGHTCYAN));
	iceImp.push_back(ColorString(R"(   /  \/`\<`)  )", ddutil::LIGHTCYAN));
	iceImp.push_back(ColorString(R"(   \ (  |\_/   )", ddutil::LIGHTCYAN));
	iceImp.push_back(ColorString(R"(   /)))-(  |   )", ddutil::LIGHTCYAN));
	iceImp.push_back(ColorString(R"(  / /^ ^ \ |   )", ddutil::LIGHTCYAN));
	iceImp.push_back(ColorString(R"( /  )^/\^( |   )", ddutil::LIGHTCYAN));
	iceImp.push_back(ColorString(R"( )_//`__>> |   )", ddutil::LIGHTCYAN));
	iceImp.push_back(ColorString(R"(   #   #`  |   )", ddutil::LIGHTCYAN));
	return iceImp;
}

Picture Art::getFairy()
{
	Picture fairy;
	fairy.push_back(ColorString(R"(.'.         .'.)", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"(|  \       /  |)", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"('.  \  |  /  .')", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"(  '. \\|// .'  )", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"(    '-- --'    )", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"(    .'/|\'.    )", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"(   '..'|'..'   )", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"(               )", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"(               )", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"(               )", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"(               )", ddutil::LIGHTMAGENTA));
	fairy.push_back(ColorString(R"(               )", ddutil::LIGHTMAGENTA));
	return fairy;
}

Picture Art::getFireDragon()
{
	Picture dragon;
	dragon.push_back(ColorString(R"(             ,  ,             )", ddutil::RED));
	dragon.push_back(ColorString(R"(             \\ \\            )", ddutil::RED));
	dragon.push_back(ColorString(R"(             ) \\ \\    _p_   )", ddutil::RED));
	dragon.push_back(ColorString(R"(             )^\))\))  /  *\  )", ddutil::RED));
	dragon.push_back(ColorString(R"(              \_|| || / /^`-' )", ddutil::RED));
	dragon.push_back(ColorString(R"(     __       -\ \\--/ /      )", ddutil::RED));
	dragon.push_back(ColorString(R"(   <'  \\___/   ___. )'       )", ddutil::RED));
	dragon.push_back(ColorString(R"(        `====\ )___/\\        )", ddutil::RED));
	dragon.push_back(ColorString(R"(             //     `"        )", ddutil::RED));
	dragon.push_back(ColorString(R"(             \\               )", ddutil::RED));
	dragon.push_back(ColorString(R"(                              )", ddutil::RED));
	dragon.push_back(ColorString(R"(                              )", ddutil::RED));
	dragon.push_back(ColorString(R"(                              )", ddutil::RED));
	dragon.push_back(ColorString(R"(                              )", ddutil::RED));
	dragon.push_back(ColorString(R"(                              )", ddutil::RED));
	dragon.push_back(ColorString(R"(                              )", ddutil::RED));
	return dragon;
}

Picture Art::getIceDragon()
{
	Picture dragon;
	dragon.push_back(ColorString(R"(             ,  ,             )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(             \\ \\            )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(             ) \\ \\    _p_   )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(             )^\))\))  /  *\  )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(              \_|| || / /^`-' )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(     __       -\ \\--/ /      )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(   <'  \\___/   ___. )'       )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(        `====\ )___/\\        )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(             //     `"        )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(             \\               )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(                              )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(                              )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(                              )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(                              )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(                              )", ddutil::LIGHTCYAN));
	dragon.push_back(ColorString(R"(                              )", ddutil::LIGHTCYAN));
	return dragon;
}

Picture Art::getSpiritKnight()
{
	Picture knight;
	knight.push_back(ColorString(R"(     /'                           )", ddutil::WHITE));
	knight.push_back(ColorString(R"(     ||                           )", ddutil::WHITE));
	knight.push_back(ColorString(R"(     ||      ** *                 )", ddutil::WHITE));
	knight.push_back(ColorString(R"(     ||      __X_                 )", ddutil::WHITE));
	knight.push_back(ColorString(R"(     ||     ( ___\                )", ddutil::WHITE));
	knight.push_back(ColorString(R"(     ||     |:  \\                )", ddutil::WHITE));
	knight.push_back(ColorString(R"(    ><><  ___)..:/_#__,           )", ddutil::WHITE));
	knight.push_back(ColorString(R"(    (X|) (|+(____)+\ _)           )", ddutil::WHITE));
	knight.push_back(ColorString(R"(     o|_\/>> + + + << \           )", ddutil::WHITE));
	knight.push_back(ColorString(R"(       |:\/|+ + + +| \_\<         )", ddutil::WHITE));
	knight.push_back(ColorString(R"(       \./  XXXXXX.  (o_)_        )", ddutil::WHITE));
	knight.push_back(ColorString(R"(           /+ + + |   \:|         )", ddutil::WHITE));
	knight.push_back(ColorString(R"(          /+ +/+ +|  -/->>>----.  )", ddutil::WHITE));
	knight.push_back(ColorString(R"(         /+ +|+ /XX /   _--,  _ \ )", ddutil::WHITE));
	knight.push_back(ColorString(R"(        \+ + + /  |X   (,\- \/_ , )", ddutil::WHITE));
	knight.push_back(ColorString(R"(        /\+ + /\  |X \    /,//_/  )", ddutil::WHITE));
	knight.push_back(ColorString(R"(       +_+_+_( )o_)X  \  (( ///   )", ddutil::WHITE));
	knight.push_back(ColorString(R"(        (_o(  /__/ X   \  \\//    )", ddutil::WHITE));
	knight.push_back(ColorString(R"(         \_|  |_/  X    \ ///     )", ddutil::WHITE));
	knight.push_back(ColorString(R"(         \_| >(_/        \,/      )", ddutil::WHITE));
	knight.push_back(ColorString(R"(    ,////__o\ /__////,    V       )", ddutil::WHITE));
	return knight;
}

Picture Art::getSamurai(int color)
{
	Picture samurai;
	samurai.push_back(ColorString(R"(                                    /))", color));
	samurai.push_back(ColorString(R"(                                   // )", color));
	samurai.push_back(ColorString(R"(                 __*_             //  )", color));
	samurai.push_back(ColorString(R"(              /-(____)           //   )", color));
	samurai.push_back(ColorString(R"(             ////- -|\          //    )", color));
	samurai.push_back(ColorString(R"(          ,____o% -,_          //     )", color));
	samurai.push_back(ColorString(R"(         /  \\   |||  ;       //      )", color));
	samurai.push_back(ColorString(R"(        /____\....::./\      //       )", color));
	samurai.push_back(ColorString(R"(       _/__/#\_ _,,_/--\    //        )", color));
	samurai.push_back(ColorString(R"(       /___/######## \/""-(\</        )", color));
	samurai.push_back(ColorString(R"(      _/__/ '#######  ""^(/</         )", color));
	samurai.push_back(ColorString(R"(    __/ /   ,)))=:=(,    //.          )", color));
	samurai.push_back(ColorString(R"(   |,--\   /Q...... /.  (/            )", color));
	samurai.push_back(ColorString(R"(   /       .Q....../..\               )", color));
	samurai.push_back(ColorString(R"(          /.Q ..../...\               )", color));
	samurai.push_back(ColorString(R"(         /......./.....\              )", color));
	samurai.push_back(ColorString(R"(         /...../  \.....\             )", color));
	samurai.push_back(ColorString(R"(         /_.._./   \..._\             )", color));
	samurai.push_back(ColorString(R"(          (` )      (` )              )", color));
	samurai.push_back(ColorString(R"(          | /        \ |              )", color));
	samurai.push_back(ColorString(R"(          '(          )'              )", color));
	samurai.push_back(ColorString(R"(         /+|          |+\             )", color));
	samurai.push_back(ColorString(R"(         |,/          \,/             )", color));
	return samurai;
}

Picture Art::getDragonSamurai()
{
	Picture samurai;
	int color1 = ddutil::DARKGRAY;
	int color2 = ddutil::LIGHTRED;
	samurai.push_back(ColorString(R"(                                    /))", color1));
	samurai.push_back(ColorString(R"(                                   // )", color2));
	samurai.push_back(ColorString(R"(                 __*_             //  )", color1));
	samurai.push_back(ColorString(R"(              /-(____)           //   )", color2));
	samurai.push_back(ColorString(R"(             ////- -|\          //    )", color1));
	samurai.push_back(ColorString(R"(          ,____o% -,_          //     )", color2));
	samurai.push_back(ColorString(R"(         /  \\   |||  ;       //      )", color1));
	samurai.push_back(ColorString(R"(        /____\....::./\      //       )", color2));
	samurai.push_back(ColorString(R"(       _/__/#\_ _,,_/--\    //        )", color1));
	samurai.push_back(ColorString(R"(       /___/######## \/""-(\</        )", color2));
	samurai.push_back(ColorString(R"(      _/__/ '#######  ""^(/</         )", color1));
	samurai.push_back(ColorString(R"(    __/ /   ,)))=:=(,    //.          )", color2));
	samurai.push_back(ColorString(R"(   |,--\   /Q...... /.  (/            )", color1));
	samurai.push_back(ColorString(R"(   /       .Q....../..\               )", color2));
	samurai.push_back(ColorString(R"(          /.Q ..../...\               )", color1));
	samurai.push_back(ColorString(R"(         /......./.....\              )", color2));
	samurai.push_back(ColorString(R"(         /...../  \.....\             )", color1));
	samurai.push_back(ColorString(R"(         /_.._./   \..._\             )", color2));
	samurai.push_back(ColorString(R"(          (` )      (` )              )", color1));
	samurai.push_back(ColorString(R"(          | /        \ |              )", color2));
	samurai.push_back(ColorString(R"(          '(          )'              )", color1));
	samurai.push_back(ColorString(R"(         /+|          |+\             )", color2));
	samurai.push_back(ColorString(R"(         |,/          \,/             )", color1));
	return samurai;
}

Picture Art::getCosmicSamurai()
{
	Picture samurai;
	int color1 = ddutil::COSMIC_COLOR;
	int color2 = ddutil::LIGHTMAGENTA;
	samurai.push_back(ColorString(R"(                                    /))", color1));
	samurai.push_back(ColorString(R"(                                   // )", color2));
	samurai.push_back(ColorString(R"(                 __*_             //  )", color1));
	samurai.push_back(ColorString(R"(              /-(____)           //   )", color2));
	samurai.push_back(ColorString(R"(             ////- -|\          //    )", color1));
	samurai.push_back(ColorString(R"(          ,____o% -,_          //     )", color2));
	samurai.push_back(ColorString(R"(         /  \\   |||  ;       //      )", color1));
	samurai.push_back(ColorString(R"(        /____\....::./\      //       )", color2));
	samurai.push_back(ColorString(R"(       _/__/#\_ _,,_/--\    //        )", color1));
	samurai.push_back(ColorString(R"(       /___/######## \/""-(\</        )", color2));
	samurai.push_back(ColorString(R"(      _/__/ '#######  ""^(/</         )", color1));
	samurai.push_back(ColorString(R"(    __/ /   ,)))=:=(,    //.          )", color2));
	samurai.push_back(ColorString(R"(   |,--\   /Q...... /.  (/            )", color1));
	samurai.push_back(ColorString(R"(   /       .Q....../..\               )", color2));
	samurai.push_back(ColorString(R"(          /.Q ..../...\               )", color1));
	samurai.push_back(ColorString(R"(         /......./.....\              )", color2));
	samurai.push_back(ColorString(R"(         /...../  \.....\             )", color1));
	samurai.push_back(ColorString(R"(         /_.._./   \..._\             )", color2));
	samurai.push_back(ColorString(R"(          (` )      (` )              )", color1));
	samurai.push_back(ColorString(R"(          | /        \ |              )", color2));
	samurai.push_back(ColorString(R"(          '(          )'              )", color1));
	samurai.push_back(ColorString(R"(         /+|          |+\             )", color2));
	samurai.push_back(ColorString(R"(         |,/          \,/             )", color1));
	return samurai;
}

Picture Art::getWhiteBackgroundSamurai()
{
	Picture samurai;
	int color = ddutil::getColor(ddutil::SAMURAI_COLOR, ddutil::WHITE);
	samurai.push_back(ColorString(R"(                                    /))", color));
	samurai.push_back(ColorString(R"(                                   // )", color));
	samurai.push_back(ColorString(R"(                 __*_             //  )", color));
	samurai.push_back(ColorString(R"(              /-(____)           //   )", color));
	samurai.push_back(ColorString(R"(             ////- -|\          //    )", color));
	samurai.push_back(ColorString(R"(          ,____o% -,_          //     )", color));
	samurai.push_back(ColorString(R"(         /  \\   |||  ;       //      )", color));
	samurai.push_back(ColorString(R"(        /____\....::./\      //       )", color));
	samurai.push_back(ColorString(R"(       _/__/#\_ _,,_/--\    //        )", color));
	samurai.push_back(ColorString(R"(       /___/######## \/""-(\</        )", color));
	samurai.push_back(ColorString(R"(      _/__/ '#######  ""^(/</         )", color));
	samurai.push_back(ColorString(R"(    __/ /   ,)))=:=(,    //.          )", color));
	samurai.push_back(ColorString(R"(   |,--\   /Q...... /.  (/            )", color));
	samurai.push_back(ColorString(R"(   /       .Q....../..\               )", color));
	samurai.push_back(ColorString(R"(          /.Q ..../...\               )", color));
	samurai.push_back(ColorString(R"(         /......./.....\              )", color));
	samurai.push_back(ColorString(R"(         /...../  \.....\             )", color));
	samurai.push_back(ColorString(R"(         /_.._./   \..._\             )", color));
	samurai.push_back(ColorString(R"(          (` )      (` )              )", color));
	samurai.push_back(ColorString(R"(          | /        \ |              )", color));
	samurai.push_back(ColorString(R"(          '(          )'              )", color));
	samurai.push_back(ColorString(R"(         /+|          |+\             )", color));
	samurai.push_back(ColorString(R"(         |,/          \,/             )", color));
	return samurai;
}

Picture Art::getCampfire()
{
	Picture campfire;
	campfire.push_back(ColorString(R"(   .(   )", ddutil::RED));
	campfire.push_back(ColorString(R"(  /%/\  )", ddutil::RED));
	campfire.push_back(ColorString(R"( (%(%)) )", ddutil::RED));
	campfire.push_back(ColorString(R"(.-'..`-.)", ddutil::BROWN));
	campfire.push_back(ColorString(R"(`-'.'`-')", ddutil::BROWN));
	return campfire;
}

Picture Art::getGhost()
{
	Picture ghost;
	ghost.push_back(ColorString(R"(    .-.                          )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(   .\ / `.                       )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(   :o o   :                      )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(   : -    `.                     )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(  :         ``.                  )", ddutil::WHITE));
	ghost.push_back(ColorString(R"( :             `.                )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(:  :         .   `.              )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(:   :          ` . `.            )", ddutil::WHITE));
	ghost.push_back(ColorString(R"( `.. :            `. ``;         )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(    `:;             `:'          )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(       :              `.         )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(        `.              `.     . )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(          `'`'`'`---..,___`;.-'  )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::WHITE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::WHITE));
	return ghost;
}

Picture Art::getBlueGhost()
{
	Picture ghost;
	ghost.push_back(ColorString(R"(    .-.                          )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(   .\ / `.                       )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(   :o o   :                      )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(   : -    `.                     )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(  :         ``.                  )", ddutil::BLUE));
	ghost.push_back(ColorString(R"( :             `.                )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(:  :         .   `.              )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(:   :          ` . `.            )", ddutil::BLUE));
	ghost.push_back(ColorString(R"( `.. :            `. ``;         )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(    `:;             `:'          )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(       :              `.         )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(        `.              `.     . )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(          `'`'`'`---..,___`;.-'  )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::BLUE));
	ghost.push_back(ColorString(R"(                                 )", ddutil::BLUE));
	return ghost;
}

Picture Art::getYellowGiantSnail()
{
	Picture giantSnail;

	giantSnail.push_back(ColorString(R"(       @)@)               _      _  )", ddutil::YELLOW));
	giantSnail.push_back(ColorString(R"(       _|_|          .-=.' `;-=./ `\)", ddutil::YELLOW));
	giantSnail.push_back(ColorString(R"(     _(___,`\  .-=-.'    '.  \  \  |)", ddutil::YELLOW));
	giantSnail.push_back(ColorString(R"(     \`==`  / /     '.     \  \  | /)", ddutil::YELLOW));
	giantSnail.push_back(ColorString(R"(      `,    \'--.     \    |   | |/ )", ddutil::YELLOW));
	giantSnail.push_back(ColorString(R"(        `\   \   \     |   \   / /  )", ddutil::YELLOW));
	giantSnail.push_back(ColorString(R"(          |   ',  \    |    \_/-'   )", ddutil::YELLOW));
	giantSnail.push_back(ColorString(R"(          |     ', \    \__,;  `\   )", ddutil::YELLOW));
	giantSnail.push_back(ColorString(R"(          \       '\\_,-:""""""'`   )", ddutil::YELLOW));
	giantSnail.push_back(ColorString(R"(           `""""""~'`               )", ddutil::YELLOW));

	return giantSnail;
}

Picture Art::getBurningSkull()
{
	Picture skull;
	skull.push_back(ColorString(R"(          )   )\/ ( ( (                     )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(  *  (   ((  /     ))\))  (  )    )         )", ddutil::YELLOW));
	skull.push_back(ColorString(R"((     \   )\(          |  ))( )  (|         )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(>)     ))/   |          )/  \((  ) \        )", ddutil::YELLOW));
	skull.push_back(ColorString(R"((     (      .        -.     V )/   )(    ( )", ddutil::YELLOW));
	skull.push_back(ColorString(R"( \   /     .   \            .       \))   )))", ddutil::YELLOW));
	skull.push_back(ColorString(R"(   )(      (  | |   )            .    (  /  )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(  )(    ,'))     \ /          \( `.    )    )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(  (\>  ,'/__      ))            __`.  /     )", ddutil::YELLOW));
	skull.push_back(ColorString(R"( ( \   | /  ___   ( \/     ___   \ | ( (    )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(  \.)  |/  /   \__      __/   \   \|  ))    )", ddutil::YELLOW));
	skull.push_back(ColorString(R"( .  \. |>  \      | __ |      /   <|  /     )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(      )/    \____/ :..: \____/     \ <      )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(   \ (|__  .      / ;: \          __| )  (  )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(    )\)  ~--_     --  --      _--~    /  )) )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(    (    |  ||               ||  |   (  /   )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(     \.  |  ||_             _||  |  /       )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(       > :  |  ~V+-I_I_I-+V~  |  : (.       )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(         \:  T\   _     _   /T  : ./        )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(          :    T^T T-+-T T^T    ;<          )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(           `_       -+-       _'            )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(                                            )", ddutil::YELLOW));
	skull.push_back(ColorString(R"(                                            )", ddutil::YELLOW));
	return skull;
}

Picture Art::getDeadAdventurer()
{
	Picture deadAdventurer;
	deadAdventurer.push_back(ColorString(R"(            ____                )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(     ,-'~~~~    ~~              )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(   ,'             ``~~~%%',     )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(  (                     %  Y    )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"( {                      %% I    )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"({      -                 %  `.  )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(|       ',                %  )  )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(|        |   ,..__      __. Y   )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(|    .,_./  Y     ^Y   J   )|   )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(\           |      |   |   ||   )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"( \          L_     . _ (_,.'(   )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(  \,   ,      ^^""' / |      )  )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(    \_  \          /,L]     /   )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(      '-_`-,       ` `   ./`    )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(         `-(_            )      )", ddutil::LIGHTGRAY));
	deadAdventurer.push_back(ColorString(R"(             ^^\..___,.--`      )", ddutil::LIGHTGRAY));
	return deadAdventurer;
}

Picture Art::getBrokenMirror()
{
	Picture mirror;
	mirror.push_back(ColorString(R"(       //-----------\\       )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(     //       | |   | \\     )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(   //  \__   /   \ /  | \\   )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||       \|     |  / __||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||         \    | |_/  ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||\     __  |   |/ __  ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||  \__/   \|   |_/  \_||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||  _    ___|  /  \_   ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||_/ \__/   |/_     \_/||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||          o  \      _||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||\       / |    \___/ ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||  \___/   |     \   /||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||     |   / \_    )-<_||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||    /  /     \  /    ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(   \\ /   |      _><    //   )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(   //\\   |     /   \ //\\   )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||   \\-----------//   ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"( /||\                   /||\ )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(/____\                 /____\)", BROKEN_MIRROR_COLOR));
	return mirror;
}

Picture Art::getMirror()
{
	Picture mirror;
	mirror.push_back(ColorString(R"(       //-----------\\       )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(     //               \\     )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(   //                   \\   )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(   \\                   //   )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(   //\\               //\\   )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||   \\-----------//   ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(  ||                     ||  )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"( /||\                   /||\ )", BROKEN_MIRROR_COLOR));
	mirror.push_back(ColorString(R"(/____\                 /____\)", BROKEN_MIRROR_COLOR));
	return mirror;
}

Picture Art::getMysteriousKnight()
{
	Picture mysteriousKnight;

	mysteriousKnight.push_back(ColorString(R"(      __            )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(     |  |           )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(     |--|           )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(     |  |           )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(   _.+  +._         )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(  /\,)    /\        )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"( :  `-._.'  ;       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"( |      "*--|       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"( | \        |       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"( |  `.      ;       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"( :         /|       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"( |\      -' |       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"( | `.       |       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"( :          |       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(  ;         |       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(  |      /  |       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(  :    .'   :       )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(   ;         ;      )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(   |         :      )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(   :          \     )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(  /    _      _`-._ )", MYSTERIOUS_KNIGHT_COLOR));
	mysteriousKnight.push_back(ColorString(R"(  `-.-" `-.___\"""  )", MYSTERIOUS_KNIGHT_COLOR));

	return mysteriousKnight;
}

Picture Art::getMinion()
{
	Picture minion;
	minion.push_back(ColorString(R"(              __.---.               )", MINION_COLOR));
	minion.push_back(ColorString(R"(          .'``        `.            )", MINION_COLOR));
	minion.push_back(ColorString(R"(        .'  -'''-..     `.          )", MINION_COLOR));
	minion.push_back(ColorString(R"(       /  /        '.      \        )", MINION_COLOR));
	minion.push_back(ColorString(R"(      /  / .--  .-'''`      '.      )", MINION_COLOR));
	minion.push_back(ColorString(R"(     '   |    ,---.    _      \     )", MINION_COLOR));
	minion.push_back(ColorString(R"(   .-.   \   / ,-. '-'   '.   .-.   )", MINION_COLOR));
	minion.push_back(ColorString(R"(   | :    `.' ((O))   ,-.  \  : |   )", MINION_COLOR));
	minion.push_back(ColorString(R"(   \_`     |   '-'   ((O)) | ___/   )", MINION_COLOR));
	minion.push_back(ColorString(R"(     \     \      /  '-'  / /       )", MINION_COLOR));
	minion.push_back(ColorString(R"(      \     \    /\     .' /        )", MINION_COLOR));
	minion.push_back(ColorString(R"(      /`.    '--'  '---' .'\        )", MINION_COLOR));
	minion.push_back(ColorString(R"(      /`.  `.           .' .'\      )", MINION_COLOR));
	minion.push_back(ColorString(R"(     /   `.'  ` - - - ' `.'   \     )", MINION_COLOR));
	minion.push_back(ColorString(R"(    /    /                \    \    )", MINION_COLOR));
	minion.push_back(ColorString(R"(   /   ,'                  `.   \   )", MINION_COLOR));
	minion.push_back(ColorString(R"(   \  ,'`.                .'`.  /   )", MINION_COLOR));
	minion.push_back(ColorString(R"(    `/    \              /    \'    )", MINION_COLOR));
	minion.push_back(ColorString(R"(     ,=   (              )   =,     )", MINION_COLOR));
	minion.push_back(ColorString(R"(     ,=   '\            /`   =,     )", MINION_COLOR));
	minion.push_back(ColorString(R"(     /    .'            `.    \     )", MINION_COLOR));
	minion.push_back(ColorString(R"(   .-'''  |                |  ```-. )", MINION_COLOR));
	minion.push_back(ColorString(R"(  `......'                `......'  )", MINION_COLOR));
	return minion;
}

Picture Art::getAltMinion()
{
	Picture minion;
	minion.push_back(ColorString(R"(              __.---.               )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(          .'``        `.            )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(        .'  -'''-..     `.          )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(       /  /        '.      \        )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(      /  / .--  .-'''`      '.      )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(     '   |    ,---.    _      \     )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(   .-.   \   / ,-. '-'   '.   .-.   )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(   | :    `.' ((O))   ,-.  \  : |   )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(   \_`     |   '-'   ((O)) | ___/   )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(     \     \      /  '-'  / /       )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(      \     \    /\     .' /        )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(      /`.    '--'  '---' .'\        )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(      /`.  `.           .' .'\      )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(     /   `.'  ` - - - ' `.'   \     )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(    /    /                \    \    )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(   /   ,'                  `.   \   )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(   \  ,'`.                .'`.  /   )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(    `/    \              /    \'    )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(     ,=   (              )   =,     )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(     ,=   '\            /`   =,     )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(     /    .'            `.    \     )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(   .-'''  |                |  ```-. )", ALT_MINION_COLOR));
	minion.push_back(ColorString(R"(  `......'                `......'  )", ALT_MINION_COLOR));
	return minion;
}

Picture Art::getBrute()
{
	Picture brute;
	brute.push_back(ColorString(R"(               (    )          )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(              ((((()))         )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(              |o\ /o)|         )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(              ( (  _')         )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(               (._.  /\__      )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(              ,\___,/ '  ')    )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"('.,_,,       (  .- .   .    )  )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"( \   \\     ( '        )(    ) )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(  \   \\    \.  _.__ ____( .  |)", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(   \  /\\   .(   .'  /\  '.  ) )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(    \(  \\.-' ( /    \/    \)  )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(     '  ()) _'.-|/\/\/\/\/\|   )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(         '\\ .( |\/\/\/\/\/|   )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(           '((  \    /\    /   )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(           ((((  '.__\/__.')   )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(            ((,) /   ((()   )  )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(             "..-,  (()("   /  )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(              _//.   ((() ."   )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(            //,/"     ((( ',   )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(                       ((  )   )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(                        / /    )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(                      _/,/'    )", ddutil::DARKGRAY));
	brute.push_back(ColorString(R"(                     /,/,"     )", ddutil::DARKGRAY));
	return brute;
}

Picture Art::getHyperBeast()
{
	Picture hyperbeast;

	hyperbeast.push_back(ColorString(R"(             \                  /              )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(    _________))                ((__________    )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(   /.-------./\\    \    /    //\.--------.\   )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(  //#######//##\\   ))  ((   //##\\########\\  )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"( //#######//###((  ((    ))  ))###\\########\\ )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(((#######((#####\\  \\  //  //#####))########)))", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"( \##' `###\######\\  \)(/  //######/####' `##/ )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(  )'    ``#)'  `##\`->oo<-'/##'  `(#''     `(  )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(          (       ``\`..'/''       )           )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                     \""(                      )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                      `- )                     )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                      / /                      )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                     ( /\                      )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                     /\| \                     )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                    (  \                       )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                        )                      )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                       /                       )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                      (                        )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                                               )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                                               )", HYPER_BEAST_COLOR));
	hyperbeast.push_back(ColorString(R"(                                               )", HYPER_BEAST_COLOR));

	return hyperbeast;
}

Picture Art::getGiantLizard()
{
	Picture giantLizard;
	giantLizard.push_back(ColorString(R"(      _///_,                      )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(     / ` ' '>                     )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(    o'  __/_'>                    )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(  /  _/  )_\'>                    )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"( "__/   /_/\_>                    )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(    ____/_/_/_/                   )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(   /,---, _/ /                    )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(  ""  /_/_/_/                     )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(     /_(_(_(_                 \   )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(    (   \_\_\\_               )\  )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(     \'__\_\_\_\__            ).\ )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(     //____|___\__)           )_/ )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(      |  _  \'___'_(           /' )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(      \_ (-'\'___'_\      __,'_'  )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(      __) \  \\___(_   __/.__,'   )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(  ,((,-,__\  '", __\_/. __,'      )", GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(                '"./_._._-'       )", GIANT_LIZARD_COLOR));
	return giantLizard;
}

Picture Art::getAltGiantLizard()
{
	Picture giantLizard;
	giantLizard.push_back(ColorString(R"(      _///_,                      )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(     / ` ' '>                     )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(    o'  __/_'>                    )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(  /  _/  )_\'>                    )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"( "__/   /_/\_>                    )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(    ____/_/_/_/                   )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(   /,---, _/ /                    )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(  ""  /_/_/_/                     )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(     /_(_(_(_                 \   )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(    (   \_\_\\_               )\  )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(     \'__\_\_\_\__            ).\ )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(     //____|___\__)           )_/ )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(      |  _  \'___'_(           /' )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(      \_ (-'\'___'_\      __,'_'  )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(      __) \  \\___(_   __/.__,'   )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(  ,((,-,__\  '", __\_/. __,'      )", ALT_GIANT_LIZARD_COLOR));
	giantLizard.push_back(ColorString(R"(                '"./_._._-'       )", ALT_GIANT_LIZARD_COLOR));
	return giantLizard;
}

Picture Art::getCollector()
{
	Picture guardian;
	guardian.push_back(ColorString(R"(               '-.,;;:;,                      )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(                 _;\;|\;:;,                   )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(                ) __ ' \;::,                  )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(            .--'  e   ':;;;:,           ;,    )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(           (^           ;;::;          ;;;,   )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(   _        --_.--.___,',:;::;     ,,,;:;;;,  )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(  < \        `;     |  ;:;;:;        ':;:;;;,,)", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(<`-; \__     ,;    /    ';:;;:,       ';;;'   )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(<`_   __',   ; ,  /    ::;;;:         //      )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(   `)|  \ \   ` .'      ';;:;,       //       )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(    `    \ `\  /        ;;:;;.      //__      )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(          \  `/`         ;:;  ~._,=~`   `~=,  )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(           \_|      (        ^     ^  ^ _^  \ )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(             \    _,`      / ^ ^  ^   .' `.^ ;)", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(    <`-.      '-;`       /`  ^   ^  /\    ) ^/)", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(    <'- \__..-'` ___,,,-'._ ^  ^ _.'\^`'-' ^/ )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(     `)_   ..-''`          `~~~~`    `~===~`  )", COLLECTOR_COLOR));
	guardian.push_back(ColorString(R"(    <_.-`-._\                                 )", COLLECTOR_COLOR));
	return guardian;
}

Picture Art::getVampireBat()
{
	Picture bat;
	bat.push_back(ColorString(R"(,_                    _,)", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"() '-._  ,_    _,  _.-' ()", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"()  _.-'.|\\--//|.'-._  ()", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"( )'   .'\/o\/o\/'.   `( )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(  ) .' . \====/ . '. (  )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(   )  / <<    >> \  (   )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(    '-._/``  ``\_.-'    )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(      __\\'--'//__      )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(     (((""`  `"")))     )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(                        )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(                        )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(                        )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(                        )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(                        )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(                        )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(                        )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(                        )", VAMP_BAT_COLOR));
	bat.push_back(ColorString(R"(                        )", VAMP_BAT_COLOR));
	return bat;
}

Picture Art::getVampireBatAlt()
{
	Picture bat;
	int color = Art::VAMP_BAT_ALT_COLOR;
	bat.push_back(ColorString(R"(,_                    _,)", color));
	bat.push_back(ColorString(R"() '-._  ,_    _,  _.-' ()", color));
	bat.push_back(ColorString(R"()  _.-'.|\\--//|.'-._  ()", color));
	bat.push_back(ColorString(R"( )'   .'\/o\/o\/'.   `( )", color));
	bat.push_back(ColorString(R"(  ) .' . \====/ . '. (  )", color));
	bat.push_back(ColorString(R"(   )  / <<    >> \  (   )", color));
	bat.push_back(ColorString(R"(    '-._/``  ``\_.-'    )", color));
	bat.push_back(ColorString(R"(      __\\'--'//__      )", color));
	bat.push_back(ColorString(R"(     (((""`  `"")))     )", color));
	bat.push_back(ColorString(R"(                        )", color));
	bat.push_back(ColorString(R"(                        )", color));
	bat.push_back(ColorString(R"(                        )", color));
	bat.push_back(ColorString(R"(                        )", color));
	bat.push_back(ColorString(R"(                        )", color));
	bat.push_back(ColorString(R"(                        )", color));
	bat.push_back(ColorString(R"(                        )", color));
	bat.push_back(ColorString(R"(                        )", color));
	bat.push_back(ColorString(R"(                        )", color));
	return bat;
}

Picture Art::getAbyssBeast()
{
	Picture beast;
	beast.push_back(ColorString(R"(                  .---.              )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(                 |.    \             )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(          __ _,   \:.   \            )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(      __.-_ ` (    |::   \           )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(     /__  e>  |    /::    ;          )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(  _  ` _) ,   /   /::'    |          )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"( _`\_ `""`/  (__.:::'     |          )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(('-\ \   /    `::'     (  |          )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(  (-`\`\/       .--. (  ) |          )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(      \|   ,   (    ) ) ( ;          )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(       |  /     '.  (/) | /          )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(       | /\.-'    `\ ((/ /           )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(      / / /         |  ||     ____   )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(    _/ /_ \     '-._|. `\    /.--.\  )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(  ('-/.--) '.___    \_`;    //    `) )", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(    (   .--...__`)   |`\\__//     (_\)", ABYSS_BEAST_COLOR));
	beast.push_back(ColorString(R"(       (((---....___/   '--'         )", ABYSS_BEAST_COLOR));
	return beast;
}

Picture Art::getBasilisk()
{
	Picture basilisk;
	basilisk.push_back(ColorString(R"(                                ,_       )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(             ,;;,  .-"```'-.     \\      )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(            ,;;;;,/^;/)^/)-.`.___//      )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(        <`\ <`\;;;|  \ / \  `-...'       )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(      <\) <\) |;;' \^ | , |              )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(       |` /|` /;'  |\/ =  |              )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(     <\/ <\/ /;'  / | ,  /`'-.,_         )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(      \ /;\ /;'  / / =   \  ^   `'.      )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(     _/_;/ /'   | /  "    '--:-.^  `'.   )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(  .-' <g>`"`\   / | ,    _,="/  '.    \  )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"( /\__    \\ Ss\ | /   ,="   |     \ ^  \ )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(( ( ,)   | \ `S|| | ,"      L_     |  ^ |)", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"( \) `-.  | ||  \| |     _,=.-"`    /    |)", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(       | | || ^ \/   ,="  /__  _.-'  ^ / )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(       / | //   /  ,"    |   ``"-._^_.'  )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(      /  \_/   /         '--,^     `-.   )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(     |  ^    ^;     _,==""/`     ^   `\  )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(     \ ^      |   ,"  _.-'    ^  .-.^  | )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(      \    ^  ;     (`    ^   .'`\  ) ^| )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(       '.^    \   ,}` ^__,.-'`\ ^ '' ^ / )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(         '-.^__\    \'`        `-.^_.-'  )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(           /   {     }                   )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(           `//"`.___.'                   )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(       ,==='`=,__||___                   )", BASILISK_COLOR));
	basilisk.push_back(ColorString(R"(        ,=`=``(,=,=.-=;                  )", BASILISK_COLOR));
	return basilisk;
}

Picture Art::getSphynx()
{
	Picture sphynx;
	sphynx.push_back(ColorString(R"(                      .sSSSSSSSs  )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(                      sSS=""^^^"s )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(          /\       , /  \_\_\|_/_))", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(         /';;     /| \\\/.-. .-./ )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(        / \;|    /. \,S'  -   - | )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(       / -.;|    | '.SS     _|  ; )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(      ; '-.;\,   |'-.SS\   __  /S )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(      | _  ';\\.  \' SSS\_____/SS )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(      |  '- ';\\.  \_SSS[_____]SS )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(      \ '--.-';;-. __SSS/\    SSS )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(       \  .--' ';;'.=SSS`\\_\_SSS )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(        `._ .-'` _';;..=.=.=.\.=\ )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(           ;-._-'  _.;\.=.=.=.|.=|)", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"( ,     _.-'    `"=._  ;\=.=__/__/ )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"( )\ .'`   __        ".;|.=.=.=./  )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"( (_\   .-`  '.   |    \/=.=.=/`   )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(  /\\         \-,|     |.--'|     )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"( /  \`,       //  \    | |  |     )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"(( (__) )  _.-'--,  \   | |  '--,  )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"( ;----' -'--,__}}}  \  '--, __}}} )", SPHYNX_COLOR));
	sphynx.push_back(ColorString(R"( \_________}}}       \___}}}      )", SPHYNX_COLOR));
	return sphynx;
}

Picture Art::getGoblin()
{
	Picture goblin;
	goblin.push_back(ColorString(R"(          ,     _.._     ,                 )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(         (`._."`    `"._.')                )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(          '._          _.'               /\)", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(           | /`-.  .-'\ | __           .'.')", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(           |(_()_\/_()_)|'  `\        ( (  )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(           ;   ,____,   ;     \        ) ) )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(            \  /VvvV\  /    \  \.__   / /  )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(           /`'._`""`_.'      \  \  `\/ /   )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(          /  .  `--'          \  \    /    )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(         /  / `-,       _.----'   \  ;     )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(        /  /     )     /  .--------` \     )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(       /  /.----'     /  /   ___.     \    )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(      /  /|   _    _,|  (---'   \     |    )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(     /  / |   \`""`  \\\\        \    |    )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(    / /`  |    |      \\\`        \   \    )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(   / /    ;    |                  /   /    )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(  / / _    \  /                 /`  /`     )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"( / _\/(    | |                 /  .'_      )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(| ( \  '--'  \               .'  (__)`\    )", GOBLIN_COLOR));
	goblin.push_back(ColorString(R"(\\\\  `-------'             /________.'    )", GOBLIN_COLOR));
	return goblin;
}

Picture Art::getLavaBeast()
{
	Picture lavaBeast;
	lavaBeast.push_back(ColorString(R"(                         _   _       _a_a     )", LAVA_BEAST_COLOR));
	lavaBeast.push_back(ColorString(R"(              _   _     _{.`=`.}_    {/ ''\_  )", LAVA_BEAST_COLOR));
	lavaBeast.push_back(ColorString(R"(        _    {.`'`.}   {.'  _  '.}  {|  ._oo) )", LAVA_BEAST_COLOR));
	lavaBeast.push_back(ColorString(R"(       { \  {/ .-. \} {/  .' '.  \} {/  |     )", LAVA_BEAST_COLOR));
	lavaBeast.push_back(ColorString(R"(~^`^~`~^~`~^~`~^~`~^~^~`^~^~`^~^~^~^~^~^~`^~~`)", ddutil::RED));
	return lavaBeast;
}

Picture Art::getFloatingMouth()
{
	Picture mouth;
	mouth.push_back(ColorString(R"(                _._    _._                )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(             .-':::`--':::`-.             )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(           .'::::::::::::::::`.           )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(         .'::::::::::::::::::::`.         )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(       .'::::::::::::::::::::::::`.       )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(     .'::::::________________::::::`.     )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(   .':::.---' /\    /\    /\ `---.:::`.   )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"( .':::.'@@\  /@@\  /@@\  /@@\  /@@`.:::`. )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(<----:@@@@@\/@@@@\/@@@@\/@@@@\/@@@@@:---->)", MOUTH_COLOR));
	mouth.push_back(ColorString(R"( `.:::`-.@@/\@@@@/\@@@@/\@@@@/\@@.-':::.' )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(   `.::::`-. \@@/  \@@/  \@@/ .-'::::.'   )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(     `.:::::`-\/____\/____\/-':::::.'     )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(       `.::::::::::::::::::::::::.'       )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(         `.::::::::::::::::::::.'         )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(           `-.::::::::::::::.-'           )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(              `-.________.-'              )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(                                          )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(                                          )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(                                          )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(                                          )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(                                          )", MOUTH_COLOR));
	mouth.push_back(ColorString(R"(                                          )", MOUTH_COLOR));
	return mouth;
}

Picture Art::getMace()
{
	Picture mace;
	mace.push_back(ColorString(R"(         |\                        )", MACE_COLOR));
	mace.push_back(ColorString(R"(         | \        /|             )", MACE_COLOR));
	mace.push_back(ColorString(R"(         |  \____  / |             )", MACE_COLOR));
	mace.push_back(ColorString(R"(        /|__/AMMA\/  |             )", MACE_COLOR));
	mace.push_back(ColorString(R"(      /AMMMMMMMMMMM\_|             )", MACE_COLOR));
	mace.push_back(ColorString(R"(  ___/AMMMMMMMMMMMMMMA             )", MACE_COLOR));
	mace.push_back(ColorString(R"(  \   |MVKMMM/ .\MMMMM\            )", MACE_COLOR));
	mace.push_back(ColorString(R"(   \__/MMMMMM\  /MMMMMM---         )", MACE_COLOR));
	mace.push_back(ColorString(R"(   |MMMMMMMMMMMMMMMMMM|  /         )", MACE_COLOR));
	mace.push_back(ColorString(R"(   |MMMM/. \MM.--MMMMMM\/          )", MACE_COLOR));
	mace.push_back(ColorString(R"(   /\MMM\  /MM\  |MMMMMM   ___     )", MACE_COLOR));
	mace.push_back(ColorString(R"(  /  |MMMMMMMMM\ |MMMMMM--/   \-.  )", MACE_COLOR));
	mace.push_back(ColorString(R"( /___/MMMMMMMMMM\|M.--M/___/_|   \ )", MACE_COLOR));
	mace.push_back(ColorString(R"(      \VMM/\MMMMMMM\  |      /\ \/ )", MACE_COLOR));
	mace.push_back(ColorString(R"(       \V/  \MMMMMMM\ |     /_  /  )", MACE_COLOR));
	mace.push_back(ColorString(R"(         |  /MMMV'   \|    |/ _/   )", MACE_COLOR));
	mace.push_back(ColorString(R"(         | /              _/  /    )", MACE_COLOR));
	mace.push_back(ColorString(R"(         |/              /| \'     )", MACE_COLOR));
	mace.push_back(ColorString(R"(                        /_  /      )", MACE_COLOR));
	mace.push_back(ColorString(R"(                        /  /       )", MACE_COLOR));
	return mace;;
}

Picture Art::getTheProtector()
{
	Picture collector;
	collector.push_back(ColorString(R"( <>=======()                           )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"((/\___   /|\\          ()==========<>_ )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(      \_/ | \\        //|\   ______/ \))", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(        \_|  \\      // | \_/          )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(          \|\/|\_   //  /\/            )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(           (oo)\ \_//  /               )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(          //_/\_\/ /  |                )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(         @@/  |=\  \  |                )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(              \_=\_ \ |                )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(                \==\ \|\_              )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(             __(\===\(  )\             )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(            (((~) __(_/   |            )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(                 (((~) \  /            )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(                 ______/ /             )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(                 '------'              )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(                                       )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(                                       )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(                                       )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(                                       )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(                                       )", PROTECTOR_COLOR));
	collector.push_back(ColorString(R"(                                       )", PROTECTOR_COLOR));
	return collector;
}



Picture Art::getPatriarch()
{
	Picture patriarch;
	patriarch.push_back(ColorString(R"(                            ,-.                            )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(       ___,---.__          /'|`\          __,---,___       )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(    ,-'    \`    `-.____,-'  |  `-.____,-'    //    `-.    )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(  ,'        |           ~'\     /`~           |        `.  )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"( /      ___//              `. ,'          ,  , \___      \ )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(|    ,-'   `-.__   _         |        ,    __,-'   `-.    |)", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(|   /          /\_  `   .    |    ,      _/\          \   |)", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(\  |           \ \`-.___ \   |   / ___,-'/ /           |  /)", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"( \  \           | `._   `\\  |  //'   _,' |           /  / )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(  `-.\         /'  _ `---'' , . ``---' _  `\         /,-'  )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(     ``       /     \    ,='/ \`=.    /     \       ''     )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(             |__   /|\_,--.,-.--,--._/|\   __|             )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(             /  `./  \\`\ |  |  | /,//' \,'  \             )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(            /   /     ||--+--|--+-/-|     \   \            )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(           |   |     /'\_\_\ | /_/_/`\     |   |           )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(            \   \__, \_     `~'     _/ .__/   /            )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(             `-._,-'   `-._______,-'   `-._,-'             )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(                                                           )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(                                                           )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(                                                           )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(                                                           )", PATRIARCH_COLOR));
	patriarch.push_back(ColorString(R"(                                                           )", PATRIARCH_COLOR));
	return patriarch;
}

Picture Art::getDist5Patriarch()
{
	Picture patriarch;
	int color = DIST5_PATRIARCH_COLOR;
	int eyeColor = ddutil::RED;

	patriarch.push_back(ColorString(R"(                            ,-.                            )", color));
	patriarch.push_back(ColorString(R"(       ___,---.__          /'|`\          __,---,___       )", color));
	patriarch.push_back(ColorString(R"(    ,-'    \`    `-.____,-'  |  `-.____,-'    //    `-.    )", color));
	patriarch.push_back(ColorString(R"(  ,'        |           ~'\     /`~           |        `.  )", color));
	patriarch.push_back(ColorString(R"( /      ___//              `. ,'          ,  , \___      \ )", color));
	patriarch.push_back(ColorString(R"(|    ,-'   `-.__   _         |        ,    __,-'   `-.    |)", color));
	patriarch.push_back(ColorString(R"(|   /          /\_  `   .    |    ,      _/\          \   |)", color));
	patriarch.push_back(ColorString(R"(\  |           \)", color)+ColorString(R"( \`-.___ \)",eyeColor)+ColorString(R"(   |  )",color)+ColorString(R"( / ___,-'/)", eyeColor)+ColorString(R"( /           |  /)", color));
	patriarch.push_back(ColorString(R"( \  \           |)",color)+ColorString(R"( `._   `\\)",eyeColor)+ColorString(R"(  | )",color)+ColorString(R"( //'   _,')",eyeColor)+ColorString(R"( |           /  / )", color));
	patriarch.push_back(ColorString(R"(  `-.\         /'  _)",color)+ColorString(R"( `---'')",eyeColor)+ColorString(R"( , .)",color)+ColorString(R"( ``---')", eyeColor)+ColorString(R"( _  `\         /,-'  )", color));
	patriarch.push_back(ColorString(R"(     ``       /     \    ,='/ \`=.    /     \       ''     )", color));
	patriarch.push_back(ColorString(R"(             |__   /|\_,--.,-.--,--._/|\   __|             )", color));
	patriarch.push_back(ColorString(R"(             /  `./  \\`\ |  |  | /,//' \,'  \             )", color));
	patriarch.push_back(ColorString(R"(            /   /     ||--+--|--+-/-|     \   \            )", color));
	patriarch.push_back(ColorString(R"(           |   |     /'\_\_\ | /_/_/`\     |   |           )", color));
	patriarch.push_back(ColorString(R"(            \   \__, \_     `~'     _/ .__/   /            )", color));
	patriarch.push_back(ColorString(R"(             `-._,-'   `-._______,-'   `-._,-'             )", color));
	patriarch.push_back(ColorString(R"(                                                           )", color));
	patriarch.push_back(ColorString(R"(                                                           )", color));
	patriarch.push_back(ColorString(R"(                                                           )", color));
	patriarch.push_back(ColorString(R"(                                                           )", color));
	patriarch.push_back(ColorString(R"(                                                           )", color));
	return patriarch;
}

Picture Art::getPortal()
{
	Picture portal;
	 
	portal.push_back(ColorString(R"(            ___________            )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(           /...........\           )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(          /. @       @ .\          )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         /.      @      .\         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         . @         @   .         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         .    @     @  @ .         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         .        @      .         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         .   @           .         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         . @      @  @   .         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         .      @      @ .         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         .   @           .         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         .      @   @    .         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         . @           @ .         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(         \.             ./         )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(          \.  @    @   ./          )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(           \.........../           )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(                                   )", PORTAL_COLOR));
	portal.push_back(ColorString(R"(                                   )", PORTAL_COLOR));

	return portal;
}

Picture Art::getPalace()
{
	Picture palace;
	palace.push_back(ColorString(R"(               )\         O_._._._A_._._._O         /(               )", PALACE_COLOR));
	palace.push_back(ColorString(R"(                \`--.___,'=================`.___,--'/                )", PALACE_COLOR));
	palace.push_back(ColorString(R"(                 \`--._.__                 __._,--'/                 )", PALACE_COLOR));
	palace.push_back(ColorString(R"(                   \  ,. l`~~~~~~~~~~~~~~~'l ,.  /                   )", PALACE_COLOR));
	palace.push_back(ColorString(R"(       __            \||(_)!_!_!_.-._!_!_!(_)||/            __       )", PALACE_COLOR));
	palace.push_back(ColorString(R"(       \\`-.__        ||_|____!!_|;|_!!____|_||        __,-'//       )", PALACE_COLOR));
	palace.push_back(ColorString(R"(        \\    `==---='-----------'='-----------`=---=='    //        )", PALACE_COLOR));
	palace.push_back(ColorString(R"(        | `--.                                         ,--' |        )", PALACE_COLOR));
	palace.push_back(ColorString(R"(         \  ,.`~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~',.  /         )", PALACE_COLOR));
	palace.push_back(ColorString(R"(           \||  ____,-------._,-------._,-------.____  ||/           )", PALACE_COLOR));
	palace.push_back(ColorString(R"(            ||\|___!`======="!`======="!`======="!___|/||            )", PALACE_COLOR));
	palace.push_back(ColorString(R"(            || |---||--------||-| | |-!!--------||---| ||            )", PALACE_COLOR));
	palace.push_back(ColorString(R"(  __O_____O_ll_lO_____O_____O|| |'|'| ||O_____O_____Ol_ll_O_____O__  )", PALACE_COLOR));
	palace.push_back(ColorString(R"(  o H o o H o o H o o H o o |-----------| o o H o o H o o H o o H o  )", PALACE_COLOR));
	palace.push_back(ColorString(R"( ___H_____H_____H_____H____O =========== O____H_____H_____H_____H___ )", PALACE_COLOR));
	palace.push_back(ColorString(R"(                          /|=============|\                          )", PALACE_COLOR));
	palace.push_back(ColorString(R"(()______()______()______() '==== +-+ ====' ()______()______()______())", PALACE_COLOR));
	palace.push_back(ColorString(R"(||{_}{_}||{_}{_}||{_}{_}/| ===== |_| ===== |\{_}{_}||{_}{_}||{_}{_}||)", PALACE_COLOR));
	palace.push_back(ColorString(R"(||      ||      ||     / |==== s(   )s ====| \     ||      ||      ||)", PALACE_COLOR));
	palace.push_back(ColorString(R"(======================()  =================  ()======================)", PALACE_COLOR));
	palace.push_back(ColorString(R"(----------------------/| ------------------- |\----------------------)", PALACE_COLOR));
	return palace;
}

Picture Art::getLaughingMask()
{
	Picture mask;
	mask.push_back(ColorString(R"( ,;/       \`. )", MASK_COLOR));
	mask.push_back(ColorString(R"(::(         ) :)", MASK_COLOR));
	mask.push_back(ColorString(R"(|:::._____,'  |)", MASK_COLOR));
	mask.push_back(ColorString(R"(|:::::::      |)", MASK_COLOR));
	mask.push_back(ColorString(R"(|:::::::  _   |)", MASK_COLOR));
	mask.push_back(ColorString(R"(|:<)_(>:<)_(> |)", MASK_COLOR));
	mask.push_back(ColorString(R"(|::::::|      |)", MASK_COLOR));
	mask.push_back(ColorString(R"(|::::::|      |)", MASK_COLOR));
	mask.push_back(ColorString(R"(:::|.`:|,'/|  :)", MASK_COLOR));
	mask.push_back(ColorString(R"(:::| `-'-' |  ;)", MASK_COLOR));
	mask.push_back(ColorString(R"( \::       ; / )", MASK_COLOR));
	mask.push_back(ColorString(R"(  \:\     / /  )", MASK_COLOR));
	mask.push_back(ColorString(R"(   \::-.-' /   )", MASK_COLOR));
	mask.push_back(ColorString(R"(    `::| ,'    )", MASK_COLOR));
	mask.push_back(ColorString(R"(      `:'      )", MASK_COLOR));
	mask.push_back(ColorString(R"(               )", MASK_COLOR));
	mask.push_back(ColorString(R"(               )", MASK_COLOR));
	mask.push_back(ColorString(R"(               )", MASK_COLOR));
	mask.push_back(ColorString(R"(               )", MASK_COLOR));
	mask.push_back(ColorString(R"(               )", MASK_COLOR));
	mask.push_back(ColorString(R"(               )", MASK_COLOR));
	mask.push_back(ColorString(R"(               )", MASK_COLOR));
	return mask;
}

Picture Art::getDevilishMask()
{
	Picture mask;
	mask.push_back(ColorString(R"( ,'/       \`. )", DMASK_COLOR));
	mask.push_back(ColorString(R"(: (         ) :)", DMASK_COLOR));
	mask.push_back(ColorString(R"(|  `._____,'  |)", DMASK_COLOR));
	mask.push_back(ColorString(R"(|             |)", DMASK_COLOR));
	mask.push_back(ColorString(R"(|   _     _   |)", DMASK_COLOR));
	mask.push_back(ColorString(R"(| <)_(> <)_(> |)", DMASK_COLOR));
	mask.push_back(ColorString(R"(|      |      |)", DMASK_COLOR));
	mask.push_back(ColorString(R"(|      |      |)", DMASK_COLOR));
	mask.push_back(ColorString(R"(:  |.`.|,'/|  :)", DMASK_COLOR));
	mask.push_back(ColorString(R"(:  | \,`./ |  ;)", DMASK_COLOR));
	mask.push_back(ColorString(R"( \ :       ; / )", DMASK_COLOR));
	mask.push_back(ColorString(R"(  \ \/\_/\/ /  )", DMASK_COLOR));
	mask.push_back(ColorString(R"(   \ `---' /   )", DMASK_COLOR));
	mask.push_back(ColorString(R"(    `.   ,'    )", DMASK_COLOR));
	mask.push_back(ColorString(R"(      `.'      )", DMASK_COLOR));
	mask.push_back(ColorString(R"(               )", DMASK_COLOR));
	mask.push_back(ColorString(R"(               )", DMASK_COLOR));
	mask.push_back(ColorString(R"(               )", DMASK_COLOR));
	mask.push_back(ColorString(R"(               )", DMASK_COLOR));
	mask.push_back(ColorString(R"(               )", DMASK_COLOR));
	mask.push_back(ColorString(R"(               )", DMASK_COLOR));
	mask.push_back(ColorString(R"(               )", DMASK_COLOR));
	return mask;
}

Picture Art::getAncientBird()
{
	Picture ancientBird;
	ancientBird.push_back(ColorString(R"(                     _                         )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                  __~a~_                       )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                  ~~;  ~_                      )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(    _                ~  ~_                _    )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(   '_\;__._._._._._._]   ~_._._._._._.__;/_`   )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(   '(/'/'/'/'|'|'|'| (    )|'|'|'|'\'\'\'\)'   )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(   (/ / / /, | | | |(/    \) | | | ,\ \ \ \)   )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(  (/ / / / / | | | ^(/    \) ^ | | \ \ \ \ \)  )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"( (/ / / / /  ^ ^ ^   (/  \)    ^ ^  \ \ \ \ \) )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"((/ / / / ^          / (||)|          ^ \ \ \ \))", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(^ / / ^            M  /||\M             ^ \ \ ^)", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"( ^ ^                  /||\                 ^ ^ )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                     //||\\                    )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                     //||\\                    )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                     //||\\                    )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                     '/||\'                    )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                                               )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                                               )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                                               )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                                               )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                                               )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                                               )", ANC_BIRD_COLOR));
	ancientBird.push_back(ColorString(R"(                                               )", ANC_BIRD_COLOR));
	return ancientBird;
}

Picture Art::getBloodAltar()
{
	Picture altar;
	int color = BLOOD_ALTAR_COLOR;
	altar.push_back(ColorString(R"(       .       )", color));
	altar.push_back(ColorString(R"(       |       )", color));
	altar.push_back(ColorString(R"(  .   ]#[   .  )", color));
	altar.push_back(ColorString(R"(   \_______/   )", color));
	altar.push_back(ColorString(R"(.    ]###[    .)", color));
	altar.push_back(ColorString(R"( \__]#.-.#[__/ )", color));
	altar.push_back(ColorString(R"(  |___| |___|  )", color));
	altar.push_back(ColorString(R"(  |___|_|___|  )", color));
	altar.push_back(ColorString(R"(  ####/_\####  )", color));
	altar.push_back(ColorString(R"(     |___|     )", color));
	altar.push_back(ColorString(R"(    /_____\    )", color));
	return altar;
}

Picture Art::getGoldAltar()
{
	Picture altar;
	int color = GOLD_ALTAR_COLOR;
	altar.push_back(ColorString(R"(       .       )", color));
	altar.push_back(ColorString(R"(       |       )", color));
	altar.push_back(ColorString(R"(  .   ]#[   .  )", color));
	altar.push_back(ColorString(R"(   \_______/   )", color));
	altar.push_back(ColorString(R"(.    ]###[    .)", color));
	altar.push_back(ColorString(R"( \__]#.-.#[__/ )", color));
	altar.push_back(ColorString(R"(  |___| |___|  )", color));
	altar.push_back(ColorString(R"(  |___|_|___|  )", color));
	altar.push_back(ColorString(R"(  ####/_\####  )", color));
	altar.push_back(ColorString(R"(     |___|     )", color));
	altar.push_back(ColorString(R"(    /_____\    )", color));
	return altar;
}

Picture Art::getSpider()
{
	Picture spider;
	int color = Art::SPIDER_COLOR;
	spider.push_back(ColorString(R"(    ____                      ,      )", color));
	spider.push_back(ColorString(R"(   /---.'.__             ____//      )", color));
	spider.push_back(ColorString(R"(        '--.\           /.---'       )", color));
	spider.push_back(ColorString(R"(   _______  \\         //            )", color));
	spider.push_back(ColorString(R"(  /.------.\  \|      .'/  ______    )", color));
	spider.push_back(ColorString(R"( //  ___  \ \ ||/|\  //  _/_----.\__ )", color));
	spider.push_back(ColorString(R"(|/  /.-.\  \ \:|< >|// _/.'..\   '--')", color));
	spider.push_back(ColorString(R"(   //   \'. | \'.|.'/ /_/ /  \\      )", color));
	spider.push_back(ColorString(R"(  //     \ \_\/" ' ~\-'.-'    \\     )", color));
	spider.push_back(ColorString(R"( //       '-._| :H: |'-.__     \\    )", color));
	spider.push_back(ColorString(R"(//           (/'==='\)'-._\     ||   )", color));
	spider.push_back(ColorString(R"(||                        \\    \|   )", color));
	spider.push_back(ColorString(R"(||                         \\    '   )", color));
	spider.push_back(ColorString(R"(|/                          \\       )", color));
	spider.push_back(ColorString(R"(                             ||      )", color));
	spider.push_back(ColorString(R"(                             ||      )", color));
	spider.push_back(ColorString(R"(                             \\      )", color));
	spider.push_back(ColorString(R"(                              '      )", color));
	return spider;
}

Picture Art::getCaveBat()
{
	Picture cavebat;
	int color = Art::CAVEBAT_COLOR;
	cavebat.push_back(ColorString(R"(....._                                            )", color));
	cavebat.push_back(ColorString(R"( `.   ``-.                               .-----.._)", color));
	cavebat.push_back(ColorString(R"(   `,     `-.                          .:      /` )", color));
	cavebat.push_back(ColorString(R"(     :       `"..                 ..-``       :   )", color));
	cavebat.push_back(ColorString(R"(     /   ...--:::`n            n.`::...       :   )", color));
	cavebat.push_back(ColorString(R"(     `:``      .` ::          /  `.     ``---..:. )", color));
	cavebat.push_back(ColorString(R"(       `\    .`  ._:   .-:   ::    `.     .-``    )", color));
	cavebat.push_back(ColorString(R"(         :  :    :_\\_/: :  .::      `.  /        )", color));
	cavebat.push_back(ColorString(R"(         : /      \-../:/_.`-`         \ :        )", color));
	cavebat.push_back(ColorString(R"(         :: _.._  q` p ` /`             \|        )", color));
	cavebat.push_back(ColorString(R"(         :-`    ``(_. ..-----hh``````/-._:        )", color));
	cavebat.push_back(ColorString(R"(                     `:      ``     /     `       )", color));
	cavebat.push_back(ColorString(R"(                     E:            /              )", color));
	cavebat.push_back(ColorString(R"(                      :          _/               )", color));
	cavebat.push_back(ColorString(R"(                      :    _..-``                 )", color));
	cavebat.push_back(ColorString(R"(                      l--``                       )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	return cavebat;
}

Picture Art::getCaveBatAlt()
{
	Picture cavebat;
	int color = Art::CAVEBATALT_COLOR;
	cavebat.push_back(ColorString(R"(....._                                            )", color));
	cavebat.push_back(ColorString(R"( `.   ``-.                               .-----.._)", color));
	cavebat.push_back(ColorString(R"(   `,     `-.                          .:      /` )", color));
	cavebat.push_back(ColorString(R"(     :       `"..                 ..-``       :   )", color));
	cavebat.push_back(ColorString(R"(     /   ...--:::`n            n.`::...       :   )", color));
	cavebat.push_back(ColorString(R"(     `:``      .` ::          /  `.     ``---..:. )", color));
	cavebat.push_back(ColorString(R"(       `\    .`  ._:   .-:   ::    `.     .-``    )", color));
	cavebat.push_back(ColorString(R"(         :  :    :_\\_/: :  .::      `.  /        )", color));
	cavebat.push_back(ColorString(R"(         : /      \-../:/_.`-`         \ :        )", color));
	cavebat.push_back(ColorString(R"(         :: _.._  q` p ` /`             \|        )", color));
	cavebat.push_back(ColorString(R"(         :-`    ``(_. ..-----hh``````/-._:        )", color));
	cavebat.push_back(ColorString(R"(                     `:      ``     /     `       )", color));
	cavebat.push_back(ColorString(R"(                     E:            /              )", color));
	cavebat.push_back(ColorString(R"(                      :          _/               )", color));
	cavebat.push_back(ColorString(R"(                      :    _..-``                 )", color));
	cavebat.push_back(ColorString(R"(                      l--``                       )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	cavebat.push_back(ColorString(R"(                                                  )", color));
	return cavebat;
}

Picture Art::getAncientLizard()
{
	Picture ancientLizard;
	int color = ANC_LIZARD_COLOR;
	ancientLizard.push_back(ColorString(R"(                     /\/\/\/\/\/\               )", color));
	ancientLizard.push_back(ColorString(R"(                    /|||||||||||||\             )", color));
	ancientLizard.push_back(ColorString(R"(                  _/|||||||||||||||:_=---.._    )", color));
	ancientLizard.push_back(ColorString(R"(                  | |||||||:'''':||  '~-._  '-. )", color));
	ancientLizard.push_back(ColorString(R"(                _/| ||||'         '-._   _:    ;)", color));
	ancientLizard.push_back(ColorString(R"(                | | ||'               '~~     _;)", color));
	ancientLizard.push_back(ColorString(R"(                | '                _.=._    _-~ )", color));
	ancientLizard.push_back(ColorString(R"(             _.~                  {     '-_'    )", color));
	ancientLizard.push_back(ColorString(R"(     _.--=.-~       _.._          {_       }    )", color));
	ancientLizard.push_back(ColorString(R"( _.-~   @-,        {    '-._     _. '~==+  |    )", color));
	ancientLizard.push_back(ColorString(R"(('          }       \_      \_.=~       |  |    )", color));
	ancientLizard.push_back(ColorString(R"(`======='  /_         ~-_    )         <_oo_>   )", color));
	ancientLizard.push_back(ColorString(R"(  `-----~~/ /'===...===' +   /                  )", color));
	ancientLizard.push_back(ColorString(R"(         <_oo_>         /  //                   )", color));
	ancientLizard.push_back(ColorString(R"(                       /  //                    )", color));
	ancientLizard.push_back(ColorString(R"(                      <_oo_>                    )", color));
	return ancientLizard;
}

Picture Art::getFireBat()
{
	Picture fireBat;
	int color = FIRE_BAT_COLOR;
	fireBat.push_back(ColorString(R"(     ,*-~"`^"*u_                                _u*"^`"~-*,     )", color));
	fireBat.push_back(ColorString(R"(  p!^       /  jPw                            w9j \        ^!p  )", color));
	fireBat.push_back(ColorString(R"(w^.._      /      "\_                      _/"     \        _.^w)", color));
	fireBat.push_back(ColorString(R"(     *_   /          \_      _    _      _/         \     _*    )", color));
	fireBat.push_back(ColorString(R"(       q /           / \q   ( `--` )   p/ \          \   p      )", color));
	fireBat.push_back(ColorString(R"(       jj5****._    /    ^\_) o  o (_/^    \    _.****6jj       )", color));
	fireBat.push_back(ColorString(R"(               *_ /      "==) ;; (=="      \ _*                 )", color));
	fireBat.push_back(ColorString(R"(                 `/.w***,   /(    )\   ,***w.\"                 )", color));
	fireBat.push_back(ColorString(R"(                  ^ ilmk ^c/ )    ( \c^      ^                  )", color));
	fireBat.push_back(ColorString(R"(                          'V')_)(_('V'                          )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	return fireBat;
}

Picture Art::getFireBatVariant()
{
	Picture fireBat;
	int color = FIRE_BAT_VARIANT_COLOR;
	fireBat.push_back(ColorString(R"(     ,*-~"`^"*u_                                _u*"^`"~-*,     )", color));
	fireBat.push_back(ColorString(R"(  p!^       /  jPw                            w9j \        ^!p  )", color));
	fireBat.push_back(ColorString(R"(w^.._      /      "\_                      _/"     \        _.^w)", color));
	fireBat.push_back(ColorString(R"(     *_   /          \_      _    _      _/         \     _*    )", color));
	fireBat.push_back(ColorString(R"(       q /           / \q   ( `--` )   p/ \          \   p      )", color));
	fireBat.push_back(ColorString(R"(       jj5****._    /    ^\_) o  o (_/^    \    _.****6jj       )", color));
	fireBat.push_back(ColorString(R"(               *_ /      "==) ;; (=="      \ _*                 )", color));
	fireBat.push_back(ColorString(R"(                 `/.w***,   /(    )\   ,***w.\"                 )", color));
	fireBat.push_back(ColorString(R"(                  ^ ilmk ^c/ )    ( \c^      ^                  )", color));
	fireBat.push_back(ColorString(R"(                          'V')_)(_('V'                          )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	fireBat.push_back(ColorString(R"(                                                                )", color));
	return fireBat;
}

Picture Art::getFirePlatypus()
{
	Picture firePlatypus;
	int color = FIRE_PLATYPUS_COLOR;
	firePlatypus.push_back(ColorString(R"(           _.- ~~^^^'~- _ __ .,.- ~ ~ ~  ~  -. _              )", color));
	firePlatypus.push_back(ColorString(R"( ________,'       ::.                       _,-  ~ -.         )", color));
	firePlatypus.push_back(ColorString(R"(((      ~_\   -s-  ::                     ,'          ;,      )", color));
	firePlatypus.push_back(ColorString(R"( \\       <.._ .;;;`                     ;           }  `',   )", color));
	firePlatypus.push_back(ColorString(R"(  ``======='    _ _- _ (   }             `,          ,'\,  `, )", color));
	firePlatypus.push_back(ColorString(R"(               ((/ _ _,i   ! _ ~ - -- - _ _'_-_,_,,,'    \,  ;)", color));
	firePlatypus.push_back(ColorString(R"(                  ((((____/            (,(,(, ____>        \,')", color));
	return firePlatypus;
}

Picture Art::getSniffer()
{
	Picture sniffer;
	int color = Art::SNIFFER_COLOR;
	sniffer.push_back(ColorString(R"(                     _,-"^\ )", color));
	sniffer.push_back(ColorString(R"(                 _,-"   ,\ ))", color));
	sniffer.push_back(ColorString(R"(        __,,__,-"     ,'  d')", color));
	sniffer.push_back(ColorString(R"(    _,-"      "-,_  ,'      )", color));
	sniffer.push_back(ColorString(R"(  ,"              "<,       )", color));
	sniffer.push_back(ColorString(R"( /                   \      )", color));
	sniffer.push_back(ColorString(R"( |                    \     )", color));
	sniffer.push_back(ColorString(R"( ;     ,     ,         \    )", color));
	sniffer.push_back(ColorString(R"(  \   ( "--,/           \   )", color));
	sniffer.push_back(ColorString(R"(   \   \   /  /\   (\/)\ \  )", color));
	sniffer.push_back(ColorString(R"(    |  )  {  /  \     } \ \ )", color));
	sniffer.push_back(ColorString(R"(   /  /   | |    |   =|  | })", color));
	sniffer.push_back(ColorString(R"(  |  /    / |     ~\  |  | \)", color));
	sniffer.push_back(ColorString(R"(  J \     J \,      (_o ,/ L)", color));
	sniffer.push_back(ColorString(R"(   "'      '"             "')", color));
	return sniffer;
}

Picture Art::getSnifferVariant()
{
	Picture sniffer;
	int color = Art::SNIFFER_VARIANT_COLOR;
	sniffer.push_back(ColorString(R"(        __,,__              )", color));
	sniffer.push_back(ColorString(R"(    _,-"      "-,_          )", color));
	sniffer.push_back(ColorString(R"(  ,"              "-,       )", color));
	sniffer.push_back(ColorString(R"( /                   ",     )", color));
	sniffer.push_back(ColorString(R"( |                     \    )", color));
	sniffer.push_back(ColorString(R"( ;     ,    ,,          \   )", color));
	sniffer.push_back(ColorString(R"(  \   ( "-,/             \  )", color));
	sniffer.push_back(ColorString(R"(   \   \ \/  /~\   (\/)\  \ )", color));
	sniffer.push_back(ColorString(R"(    |  ) {  / ) \     } \  ;)", color));
	sniffer.push_back(ColorString(R"(   /  /  | | /   |   =|  | })", color));
	sniffer.push_back(ColorString(R"(  |  /   / |/     ~\  |  | \)", color));
	sniffer.push_back(ColorString(R"(  J \    J \,       (_o ,/ L)", color));
	sniffer.push_back(ColorString(R"(   "'     '"             "' )", color));
	return sniffer;
}

Picture Art::getCorruptedDisciple()
{
	Picture cdisciple;
	int color = Art::CORRUPTED_DISCIPLE_COLOR;
	cdisciple.push_back(ColorString(R"(.                                       ,)", color));
	cdisciple.push_back(ColorString(R"()).                                   ,(()", color));
	cdisciple.push_back(ColorString(R"())).                                 ,((()", color));
	cdisciple.push_back(ColorString(R"())))).            .:::.           ,(((((()", color));
	cdisciple.push_back(ColorString(R"())))))))).        :   :        ,((((((((')", color));
	cdisciple.push_back(ColorString(R"(`))))))))))).     :   :    ,(((((((((((( )", color));
	cdisciple.push_back(ColorString(R"( ))))))))))))))))_:' ':_(((((((((((((((' )", color));
	cdisciple.push_back(ColorString(R"( `)))))))))))).-' \___/ '-._(((((((((((  )", color));
	cdisciple.push_back(ColorString(R"(  `))))_._.-' __)(     )(_  '-._._(((('  )", color));
	cdisciple.push_back(ColorString(R"(   `))'---)___)))'\_ _/'((((__(---'(('   )", color));
	cdisciple.push_back(ColorString(R"(     `))))))))))))|' '|(((((((((((('     )", color));
	cdisciple.push_back(ColorString(R"(       `)))))))))/'   '\((((((((('       )", color));
	cdisciple.push_back(ColorString(R"(         `)))))))|     |((((((('         )", color));
	cdisciple.push_back(ColorString(R"(          `))))))|     |(((((('          )", color));
	cdisciple.push_back(ColorString(R"(                /'     '\                )", color));
	cdisciple.push_back(ColorString(R"(               /'       '\               )", color));
	cdisciple.push_back(ColorString(R"(              /'         '\              )", color));
	cdisciple.push_back(ColorString(R"(             /'           '\             )", color));
	cdisciple.push_back(ColorString(R"(             '---..___..---'             )", color));
	return cdisciple;
}

Picture Art::getAncientMask()
{
	Picture mask;
	int color = ANCIENT_MASK_COLOR;
	mask.push_back(ColorString(R"(         .,ad88888ba,.    )", color));
	mask.push_back(ColorString(R"(     .,ad8888888888888a,  )", color));
	mask.push_back(ColorString(R"(    d8P"""98888P"""98888b,)", color));
	mask.push_back(ColorString(R"(    9b    d8888,    `9888B)", color));
	mask.push_back(ColorString(R"(  ,d88aaa8888888b,,,d888P')", color));
	mask.push_back(ColorString(R"( d8888888888888888888888b )", color));
	mask.push_back(ColorString(R"(d888888P""98888888888888P )", color));
	mask.push_back(ColorString(R"(88888P'    9888888888888  )", color));
	mask.push_back(ColorString(R"(`98P'       9888888888P'  )", color));
	mask.push_back(ColorString(R"(             `"9888P"'    )", color));
	mask.push_back(ColorString(R"(                `"'       )", color));
	return mask;
}

Picture Art::getGiantHead()
{
	Picture head;
	int color = GIANT_HEAD_COLOR;
	head.push_back(ColorString(R"(        _.---**""**-.       )", color));
	head.push_back(ColorString(R"(._   .-'           /|`.     )", color));
	head.push_back(ColorString(R"( \`.'             / |  `.   )", color));
	head.push_back(ColorString(R"(  V              (  ;    \  )", color));
	head.push_back(ColorString(R"(  L       _.-  -. `'      \ )", color));
	head.push_back(ColorString(R"( / `-. _.'       \         ;)", color));
	head.push_back(ColorString(R"(:            __   ;    _   |)", color));
	head.push_back(ColorString(R"(:`-.___.+-*"': `  ;  .' `. |)", color));
	head.push_back(ColorString(R"(|`-/     `--*'   /  /  /`.\|)", color));
	head.push_back(ColorString(R"(: :              \    :`.| ;)", color));
	head.push_back(ColorString(R"(| |   .           ;/ .' ' / )", color));
	head.push_back(ColorString(R"(: :  / `             :__.'  )", color));
	head.push_back(ColorString(R"( \`._.-'       /     |      )", color));
	head.push_back(ColorString(R"(  : )         :      ;      )", color));
	head.push_back(ColorString(R"(  :----.._    |     /       )", color));
	head.push_back(ColorString(R"( : .-.    `.       /        )", color));
	head.push_back(ColorString(R"(  \     `._       /         )", color));
	head.push_back(ColorString(R"(  /`-            /          )", color));
	head.push_back(ColorString(R"( :             .'           )", color));
	head.push_back(ColorString(R"(  \ )       .-'             )", color));
	head.push_back(ColorString(R"(   `-----*"'                )", color));
	return head;
}

Picture Art::getBug()
{
	Picture bug;
	bug.push_back(ColorString(R"(      ,_      _,      )", BUG_COLOR));
	bug.push_back(ColorString(R"(        '.__.'        )", BUG_COLOR));
	bug.push_back(ColorString(R"(   '-,   (__)   ,-'   )", BUG_COLOR));
	bug.push_back(ColorString(R"(     '._ .::. _.'     )", BUG_COLOR));
	bug.push_back(ColorString(R"(       _'(^^)'_       )", BUG_COLOR));
	bug.push_back(ColorString(R"(    _,` `>\/<` `,_    )", BUG_COLOR));
	bug.push_back(ColorString(R"(   `  ,-` )( `-,  `   )", BUG_COLOR));
	bug.push_back(ColorString(R"(      |  /==\  |      )", BUG_COLOR));
	bug.push_back(ColorString(R"(    ,-'  |=-|  '-,    )", BUG_COLOR));
	bug.push_back(ColorString(R"(         )-=(         )", BUG_COLOR));
	bug.push_back(ColorString(R"(         \__/         )", BUG_COLOR));
	return bug;
}

Picture Art::getTheWanderer()
{
	Picture wanderer;
	int color = WANDERER_COLOR;
	wanderer.push_back(ColorString(R"(                               _..-=~=-._        )", color));
	wanderer.push_back(ColorString(R"(                          _.-~'          ~.      )", color));
	wanderer.push_back(ColorString(R"(              __..---~~~~~                 ~.    )", color));
	wanderer.push_back(ColorString(R"(         _.-~~                      _.._     ~.  )", color));
	wanderer.push_back(ColorString(R"(     _ -~_                         /    \      ; )", color));
	wanderer.push_back(ColorString(R"(    (                             {      |      :)", color));
	wanderer.push_back(ColorString(R"(    / \/                          |      |      :)", color));
	wanderer.push_back(ColorString(R"(   /  /\ /}         (  )          |      |     .-)", color));
	wanderer.push_back(ColorString(R"(  /     //-=-~-_-_  |  |          \      ;    .' )", color));
	wanderer.push_back(ColorString(R"( /     //     | =._-|  }/ / / /_.==\     ; _.'   )", color));
	wanderer.push_back(ColorString(R"(( oo  //|     = )  ~| /.__..-='|    \    :'      )", color));
	wanderer.push_back(ColorString(R"( ====||      / /    + )    \   |_.-~`\   :       )", color));
	wanderer.push_back(ColorString(R"(            / /    / /      \  |     ([ ])       )", color));
	wanderer.push_back(ColorString(R"(           /_/    / /       (  ]     `/ \'       )", color));
	wanderer.push_back(ColorString(R"(          (((|   /_/      __/_/__    -| |--      )", color));
	wanderer.push_back(ColorString(R"(                (((|      -----     __|_|__      )", color));
	wanderer.push_back(ColorString(R"(                 '''                 -----       )", color));
	return wanderer;
}

Picture Art::getTheMuncher()
{
	Picture muncher;
	int color = MUNCHER_COLOR;
	muncher.push_back(ColorString(R"(                     .--.  .--.              )", color));
	muncher.push_back(ColorString(R"(                    /    \/    \             )", color));
	muncher.push_back(ColorString(R"(                   | .-.  .-.   \            )", color));
	muncher.push_back(ColorString(R"(                   |/_  |/_  |   \           )", color));
	muncher.push_back(ColorString(R"(                   ||. \|| .\|    `----.     )", color));
	muncher.push_back(ColorString(R"(                   |\ _/ \ _/    --,    \_   )", color));
	muncher.push_back(ColorString(R"( .--"""""-.       /              (` \     `-.)", color));
	muncher.push_back(ColorString(R"(/          \_____/                           )", color));
	muncher.push_back(ColorString(R"(\  () ()                         /`\         )", color));
	muncher.push_back(ColorString(R"(|                         .___.-'   |        )", color));
	muncher.push_back(ColorString(R"(\                        /` \|      /        )", color));
	muncher.push_back(ColorString(R"( `-.___             ___.' .-.`.---.|         )", color));
	muncher.push_back(ColorString(R"(    \| ``-..___,.-'`\| / /   /     |         )", color));
	muncher.push_back(ColorString(R"(     `      \|      ,`/ /   /   ,  /         )", color));
	muncher.push_back(ColorString(R"(             `      |\ /   /    |\/          )", color));
	muncher.push_back(ColorString(R"(              ,   .'`-;   '     \/           )", color));
	muncher.push_back(ColorString(R"(         ,    |\-'  .'   ,   .-'`            )", color));
	muncher.push_back(ColorString(R"(       .-|\--;`` .-'     |\.'                )", color));
	muncher.push_back(ColorString(R"(      ( `"'-.|\ (___,.--'`'                  )", color));
	muncher.push_back(ColorString(R"(       `-.    `"`          _.--'             )", color));
	muncher.push_back(ColorString(R"(          `.          _.-'`-.                )", color));
	muncher.push_back(ColorString(R"(            `''---''``       `.              )", color));
	return muncher;
}

Picture Art::getTheArchitect()
{
	Picture architect;
	int color = ARCHITECT_COLOR;
	architect.push_back(ColorString(R"(  .:'                                  `:.  )", color));
	architect.push_back(ColorString(R"( ::'                                    `:: )", color));
	architect.push_back(ColorString(R"(:: :.                                  .: ::)", color));
	architect.push_back(ColorString(R"( `:. `:.             .             .:'  .:' )", color));
	architect.push_back(ColorString(R"(  `::. `::           !           ::' .::'   )", color));
	architect.push_back(ColorString(R"(      `::.`::.    .' ! `.    .::'.::'       )", color));
	architect.push_back(ColorString(R"(        `:.  `::::'':!:``::::'   ::'        )", color));
	architect.push_back(ColorString(R"(        :'*:::.  .:' ! `:.  .:::*`:         )", color));
	architect.push_back(ColorString(R"(       :: HHH::.   ` ! '   .::HHH ::        )", color));
	architect.push_back(ColorString(R"(      ::: `H TH::.  `!'  .::HT H' :::       )", color));
	architect.push_back(ColorString(R"(      ::..  `THHH:`:   :':HHHT'  ..::       )", color));
	architect.push_back(ColorString(R"(      `::      `T: `. .' :T'      ::'       )", color));
	architect.push_back(ColorString(R"(        `:. .   :         :   . .:'         )", color));
	architect.push_back(ColorString(R"(          `::'               `::'           )", color));
	architect.push_back(ColorString(R"(            :'  .`.  .  .'.  `:             )", color));
	architect.push_back(ColorString(R"(            :' ::.       .:: `:             )", color));
	architect.push_back(ColorString(R"(            :' `:::     :::' `:             )", color));
	architect.push_back(ColorString(R"(             `.  ``     ''  .'              )", color));
	architect.push_back(ColorString(R"(              :`...........':               )", color));
	architect.push_back(ColorString(R"(              ` :`.     .': '               )", color));
	architect.push_back(ColorString(R"(               `:  `"""'  :'                )", color));
	architect.push_back(ColorString(R"(                "         "                 )", color));
	return architect;
}

Picture Art::getTheHarbinger()
{
	Picture harbinger;
	int color = HARBINGER_COLOR;
	harbinger.push_back(ColorString(R"(             _                                           )", color));
	harbinger.push_back(ColorString(R"(            `_))  __,  _.-"""--.                         )", color));
	harbinger.push_back(ColorString(R"( __        .'   Y`_ /;'  ,;;-'`                          )", color));
	harbinger.push_back(ColorString(R"(/ \\-.,_   |e    ( //  ,'`(                       ,      )", color));
	harbinger.push_back(ColorString(R"(| ||  _ '-/_    .-'| `.;;<                        )\     )", color));
	harbinger.push_back(ColorString(R"(\_//_/ \/|-\_/ (   | ;',,/             _.-""""""-' /     )", color));
	harbinger.push_back(ColorString(R"(     \`\ \.-/   `\/  .;;/            .'       ,_.'`'.    )", color));
	harbinger.push_back(ColorString(R"(      \ \ \/   /  |  ';;|          .'    _..-.  `\   '.  )", color));
	harbinger.push_back(ColorString(R"(       \ \ `  /   |`\`;.`\        /    .'     `\  |.   \ )", color));
	harbinger.push_back(ColorString(R"(        '-\_.'    ; /| ';|   _   / '  /    .--. )/  \   ;)", color));
	harbinger.push_back(ColorString(R"(          /   ,  / (_/ ;.'  / './   .--,  /  _ `\    ;  ;)", color));
	harbinger.push_back(ColorString(R"(         |  ,;'  \  (_(     \(\ \ /`  _ \ \_( ) |    |  |)", color));
	harbinger.push_back(ColorString(R"(         |        |            | Y  /` )/    /  /    |  |)", color));
	harbinger.push_back(ColorString(R"(         \        (_           | ;  |      .'  /     |  |)", color));
	harbinger.push_back(ColorString(R"(      /( _)      ,.-'         / ,   ;     /  /`      ;  ;)", color));
	harbinger.push_back(ColorString(R"(      \     /`|  \`._      _.' ,   /     ;  |        ;  ;)", color));
	harbinger.push_back(ColorString(R"(       '---;   `"-;  '----'  ,    /      |  \       /  / )", color));
	harbinger.push_back(ColorString(R"(            \             ,     .'       \   '----'`  /  )", color));
	harbinger.push_back(ColorString(R"(             '.    ,   ,      .'          '.        .'   )", color));
	harbinger.push_back(ColorString(R"(               '-._________.-'             `""""""`      )", color));
	return harbinger;
}

Picture Art::getTheSalvager()
{
	Picture salvager;
	int color = SALVAGER_COLOR;
	salvager.push_back(ColorString(R"(                                                     ,|  )", color));
	salvager.push_back(ColorString(R"(                                                   -~.|  )", color));
	salvager.push_back(ColorString(R"(                                                _-~  /   )", color));
	salvager.push_back(ColorString(R"(                                             _-~    /_-~|)", color));
	salvager.push_back(ColorString(R"(                                         _ -~     _-~ ,/')", color));
	salvager.push_back(ColorString(R"(                                   __--~~__--\ _-~  _/,  )", color));
	salvager.push_back(ColorString(R"(                             __--~~  --~~  __/~  _-~ /   )", color));
	salvager.push_back(ColorString(R"(                            /        __--~~  \-~~ _-~    )", color));
	salvager.push_back(ColorString(R"(                           /     --~~   __--~' _-~ ~|    )", color));
	salvager.push_back(ColorString(R"(                          /     ___---~~  ~~\~~__--~     )", color));
	salvager.push_back(ColorString(R"(        _.---.           /           __--~~~'~~/         )", color));
	salvager.push_back(ColorString(R"(     .-"      "=\ __    /      ~~~~~__\__---~~_.         )", color));
	salvager.push_back(ColorString(R"(    (_ .-b=-   =:Y  ~~~/      ---~~~/__-----=~           )", color));
	salvager.push_back(ColorString(R"(   /  __      =:;l    /      ----~~/        `::\         )", color));
	salvager.push_back(ColorString(R"(  / o~  "c    =~:l    (      ---~~/     __  `:::|        )", color));
	salvager.push_back(ColorString(R"( (  .--~"~-._=~;/'___  `----~~~~'     ,'  `\  `:|        )", color));
	salvager.push_back(ColorString(R"(  \(       ( ~~ ,'  ,`\              (      `\ /         )", color));
	salvager.push_back(ColorString(R"(.=z.       `>.-~   /   )          ___.\_     `Y.         )", color));
	salvager.push_back(ColorString(R"(   \`------}    .-"  _/"~~~----~~~ __./'        )        )", color));
	salvager.push_back(ColorString(R"( .==)_.<---}_   __.-~          ,-~~         ,-~'         )", color));
	salvager.push_back(ColorString(R"(       \\    "~"              :      __.--"~             )", color));
	salvager.push_back(ColorString(R"(        ))                     `((((~~                   )", color));
	
	return salvager;
}

Picture Art::getTheBetrayer(int eyeColor)
{
	Picture betrayer;
	int color = BETRAYER_COLOR;
	betrayer.push_back(ColorString(R"(       .-""-.                   .-""-.       )", color));
	betrayer.push_back(ColorString(R"(     .'_.-.  |                 |  .-._'.     )", color));
	betrayer.push_back(ColorString(R"(    /    _/ /                   \ \_    \    )", color));
	betrayer.push_back(ColorString(R"(   /.--.' | |      ,;;;;;,      | | '.--.\   )", color));
	betrayer.push_back(ColorString(R"(  /   .-`-| |    ;//;/;/;\\;    | |-`-.   \  )", color));
	betrayer.push_back(ColorString(R"( ;.--':   | |   /;/;/;//\\;\\   | |   :'--.; )", color));
	betrayer.push_back(ColorString(R"(|    _\.'-| |  ((;(/;/; \;\);)  | |-'./_    |)", color));
	betrayer.push_back(ColorString(R"(;_.-'/:   | |  );)) )",color)+ColorString("*",eyeColor)+ColorString("   ",color)+ColorString("*",eyeColor)+ColorString(R"( (;((  | |   :\'-._;)", color));
	betrayer.push_back(ColorString(R"(|   | _:-'\  \((((    \    );))/  /'-:_ |   |)", color));
	betrayer.push_back(ColorString(R"(;  .:` '._ \  );))\   "   /((((  / _.' `:.  ;)", color));
	betrayer.push_back(ColorString(R"(|-` '-.;_ `-\(;(;((\  =  /););))/-` _;.-' `-|)", color));
	betrayer.push_back(ColorString(R"(; / .'\ |`'\ );));)/`---`\((;(((./`'| /'. \ ;)", color));
	betrayer.push_back(ColorString(R"(| .' / `'.\-((((((\       /))));) \.'` \ '. |)", color));
	betrayer.push_back(ColorString(R"(;/  /\_/-`-/ ););)|   ,   |;(;(( \` -\_/\  \;)", color));
	betrayer.push_back(ColorString(R"(|.' .| `;/   (;(|'==/|\=='|);)   \;` |. '.|  )", color));
	betrayer.push_back(ColorString(R"(|  / \.'/      / _.` | `._ \      \'./ \  |  )", color));
	betrayer.push_back(ColorString(R"( \| ; |;    _,.-` \_/Y\_/ `-.,_    ;| ; |/   )", color));
	betrayer.push_back(ColorString(R"(  \ | ;|   `       | | |       `   |. | /    )", color));
	betrayer.push_back(ColorString(R"(    `\ ||           | | |           || /`    )", color));
	betrayer.push_back(ColorString(R"(      `:_\         _\/ \/_         /_:'      )", color));
	betrayer.push_back(ColorString(R"(                                             )", color));
	betrayer.push_back(ColorString(R"(                                             )", color));
	betrayer.push_back(ColorString(R"(                                             )", color));
	return betrayer;
}

Picture Art::getDemonicDisciple()
{
	Picture cultist;
	int color = DEMONIC_DISCIPLE_COLOR;
	cultist.push_back(ColorString(R"(              _,-'|    )", color));
	cultist.push_back(ColorString(R"(           ,-'._  |    )", color));
	cultist.push_back(ColorString(R"( .||,      |####\ |    )", color));
	cultist.push_back(ColorString(R"(\.`',/     \####| |    )", color));
	cultist.push_back(ColorString(R"(=\/\/=      |###| |    )", color));
	cultist.push_back(ColorString(R"(/ || \    ,-'\#/,'`.   )", color));
	cultist.push_back(ColorString(R"(  ||     ,'   `,,. `.  )", color));
	cultist.push_back(ColorString(R"(  ,|____,' , ,;' \| |  )", color));
	cultist.push_back(ColorString(R"( (3|\    _/|/'   _| |  )", color));
	cultist.push_back(ColorString(R"(  ||/,-''  | >-'' _,\\ )", color));
	cultist.push_back(ColorString(R"(  ||'      ==\ ,-'  ,' )", color));
	cultist.push_back(ColorString(R"(  ||       |  V \ ,|   )", color));
	cultist.push_back(ColorString(R"(  ||       |    |` |   )", color));
	cultist.push_back(ColorString(R"(  ||       |    |   \  )", color));
	cultist.push_back(ColorString(R"(  ||       |    \    \ )", color));
	cultist.push_back(ColorString(R"(  ||       |     |    \)", color));
	cultist.push_back(ColorString(R"(  ||       |      \_,-')", color));
	cultist.push_back(ColorString(R"(  ||       |___,,--")_\)", color));
	cultist.push_back(ColorString(R"(  ||         |_|   ccc/)", color));
	cultist.push_back(ColorString(R"(  ||        ccc/       )", color));
	cultist.push_back(ColorString(R"(  ||                   )", color));
	cultist.push_back(ColorString(R"(                     )", color));
	return cultist;
}

Picture Art::getGiantSnail()
{
	Picture giantSnail;

	giantSnail.push_back(ColorString(R"(       @)@)               _      _  )", GIANT_SNAIL_COLOR));
	giantSnail.push_back(ColorString(R"(       _|_|          .-=.' `;-=./ `\)", GIANT_SNAIL_COLOR));
	giantSnail.push_back(ColorString(R"(     _(___,`\  .-=-.'    '.  \  \  |)", GIANT_SNAIL_COLOR));
	giantSnail.push_back(ColorString(R"(     \`==`  / /     '.     \  \  | /)", GIANT_SNAIL_COLOR));
	giantSnail.push_back(ColorString(R"(      `,    \'--.     \    |   | |/ )", GIANT_SNAIL_COLOR));
	giantSnail.push_back(ColorString(R"(        `\   \   \     |   \   / /  )", GIANT_SNAIL_COLOR));
	giantSnail.push_back(ColorString(R"(          |   ',  \    |    \_/-'   )", GIANT_SNAIL_COLOR));
	giantSnail.push_back(ColorString(R"(          |     ', \    \__,;  `\   )", GIANT_SNAIL_COLOR));
	giantSnail.push_back(ColorString(R"(          \       '\\_,-:""""""'`   )", GIANT_SNAIL_COLOR));
	giantSnail.push_back(ColorString(R"(           `""""""~'`               )", GIANT_SNAIL_COLOR));

	return giantSnail;
}

Picture Art::getDynamite()
{
	Picture dynamite;
	dynamite.push_back(ColorString(R"(      ______________________________    . \  | / .     )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"(     /                            / \     \ \ / /      )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"(    |                            | ==========  - -     )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"(     \____________________________\_/     / / \ \      )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"(  ______________________________      \  | / | \       )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"( /                            / \     \ \ / /.   .     )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"(|                            | ==========  - -         )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"( \____________________________\_/     / / \ \    /     )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"(      ______________________________   / |\  | /  .    )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"(     /                            / \     \ \ / /      )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"(    |                            | ==========  -  - -  )", DYNAMITE_COLOR));
	dynamite.push_back(ColorString(R"(     \____________________________\_/     / / \ \      )", DYNAMITE_COLOR));

	return dynamite;
}

Picture Art::getTheWatcher()
{
	Picture watcher;
	watcher.push_back(ColorString(R"(            .-.-.        )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(    . .    / %%% \       )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  '     ' / %%%%\ \      )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"('  \.'./ ( &%%%%%) )     )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(. -(( ))- '.o,� )%/\     )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"( .  /\\_ . /\-_/*%%.\    )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(   .  | /  \�.| %%.�/    )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(     _|/\ /\_\__/_/\\    )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(    (( )\\. '::::'. )\   )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  .'.\  \\\. .\/. .'\/   )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  |\| './)'\'.__.'/\_)   )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  |_|_______)_  _(  |-\  )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  (^ ^_^_^_^__\/_^\ |  \ )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  |\^/ ^ ^ ^ ^ ^ ^ \(__)))", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  |/)^ .' ( ^\^_^_.'_/|  )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  |/^ ^| ^ \ ^\ ____/ /  )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  /^ ^ |^ ^ \ ^'.___\|\  )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"( (^_^_^| ^ ^ \-^'        )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  | |  \^_^_^/           )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(  |\|_.:_\ '.\           )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(.-'-'----'--\:|--- - --  )", WATCHER_COLOR));
	watcher.push_back(ColorString(R"(|            \|          )", WATCHER_COLOR));
	return watcher;
}

Picture Art::getTheWatcherWhiteBackground()
{
	Picture watcher;
	int color = ddutil::getColor(WATCHER_COLOR, ddutil::WHITE);
	watcher.push_back(ColorString(R"(            .-.-.        )", color));
	watcher.push_back(ColorString(R"(    . .    / %%% \       )", color));
	watcher.push_back(ColorString(R"(  '     ' / %%%%\ \      )", color));
	watcher.push_back(ColorString(R"('  \.'./ ( &%%%%%) )     )", color));
	watcher.push_back(ColorString(R"(. -(( ))- '.o,� )%/\     )", color));
	watcher.push_back(ColorString(R"( .  /\\_ . /\-_/*%%.\    )", color));
	watcher.push_back(ColorString(R"(   .  | /  \�.| %%.�/    )", color));
	watcher.push_back(ColorString(R"(     _|/\ /\_\__/_/\\    )", color));
	watcher.push_back(ColorString(R"(    (( )\\. '::::'. )\   )", color));
	watcher.push_back(ColorString(R"(  .'.\  \\\. .\/. .'\/   )", color));
	watcher.push_back(ColorString(R"(  |\| './)'\'.__.'/\_)   )", color));
	watcher.push_back(ColorString(R"(  |_|_______)_  _(  |-\  )", color));
	watcher.push_back(ColorString(R"(  (^ ^_^_^_^__\/_^\ |  \ )", color));
	watcher.push_back(ColorString(R"(  |\^/ ^ ^ ^ ^ ^ ^ \(__)))", color));
	watcher.push_back(ColorString(R"(  |/)^ .' ( ^\^_^_.'_/|  )", color));
	watcher.push_back(ColorString(R"(  |/^ ^| ^ \ ^\ ____/ /  )", color));
	watcher.push_back(ColorString(R"(  /^ ^ |^ ^ \ ^'.___\|\  )", color));
	watcher.push_back(ColorString(R"( (^_^_^| ^ ^ \-^'        )", color));
	watcher.push_back(ColorString(R"(  | |  \^_^_^/           )", color));
	watcher.push_back(ColorString(R"(  |\|_.:_\ '.\           )", color));
	watcher.push_back(ColorString(R"(.-'-'----'--\:|--- - --  )", color));
	watcher.push_back(ColorString(R"(|            \|          )", color));
	return watcher;
}

Picture Art::getTreasureChest()
{
	Picture chest;
	chest.push_back(ColorString(R"(        ____...------------...____       )", TREASURE_COLOR));
	chest.push_back(ColorString(R"(   _.-"` /o/__ ____ __ __  __ \o\_`"-._  )", TREASURE_COLOR));
	chest.push_back(ColorString(R"( .'     / /                    \ \     '.)", TREASURE_COLOR));
	chest.push_back(ColorString(R"( |=====/o/======================\o\=====|)", TREASURE_COLOR));
	chest.push_back(ColorString(R"( |____/_/________..____..________\_\____|)", TREASURE_COLOR));
	chest.push_back(ColorString(R"(/   _/ \_     <_o#\__/#o_>     _/ \_   \ )", TREASURE_COLOR));
	chest.push_back(ColorString(R"(\_________\####/________\####/_________/ )", TREASURE_COLOR));
	chest.push_back(ColorString(R"( |===\!/========================\!/===|  )", TREASURE_COLOR));
	chest.push_back(ColorString(R"( |   |=|          .---.         |=|   |  )", TREASURE_COLOR));
	chest.push_back(ColorString(R"( |===|o|=========/     \========|o|===|  )", TREASURE_COLOR));
	chest.push_back(ColorString(R"( |   | |         \() ()/        | |   |  )", TREASURE_COLOR));
	chest.push_back(ColorString(R"( |===|o|======{'-.) A (.-'}=====|o|===|  )", TREASURE_COLOR));
	chest.push_back(ColorString(R"( | __/ \__     '-.\uuu/.-'    __/ \__ |  )", TREASURE_COLOR));
	chest.push_back(ColorString(R"( |==== .'.'^'.'.====|====.'.'^'.'.====|  )", TREASURE_COLOR));
	chest.push_back(ColorString(R"( |  _\o/   __  {.' __  '.} _   _\o/  _|  )", TREASURE_COLOR));
	chest.push_back(ColorString(R"( `""""-""""""""""""""""""""""""""-""""`  )", TREASURE_COLOR));
	return chest;
}
