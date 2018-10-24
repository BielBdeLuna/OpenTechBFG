/*
 * shell_interface.cpp
 *
 *  Created on: 15 d’oct. 2018
 *      Author: Biel Bestué de Luna
 */

#include <shell/shell_interface.h>

namespace BFG {

blGeneralInterface::blGeneralInterface( idDict _data ) {
	Clear();
	SetWholeData( _data );
}

blGeneralInterface::~blGeneralInterface() {
	Clear();
}

void blGeneralInterface::Clear() {
	ClearData();
}

void blGeneralInterface::ClearData() {
	data.Clear();
}

void blGeneralInterface::SetWholeData( idDict _data ) {
	ClearData();
	data = _data;
};

/*
========================
blGeneralInterface::GetKeyData
get the value of key or get the defaultValue instead, conversion is made out of this class.
========================
*/
idStr blGeneralInterface::GetKeyData( idStr key, idStr defaultValue ) {
	idStr result = defaultValue;

	if( data.FindKeyIndex( key ) != 0 ) {
		result = data.FindKey( key );
	}

	return result;
}

} /* namespace BFG */
