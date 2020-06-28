/*
 * shell_interface.h
 *
 *  Created on: 15 d’oct. 2018
 *      Author: Biel Bestué de Luna
 */

#ifndef NEO_SHELL_SHELL_INTERFACE_H_
#define NEO_SHELL_SHELL_INTERFACE_H_

#include "../idlib/Dict.h"
#include "../idlib/Str.h"

namespace BFG {

class blGeneralInterface {
public:
	blGeneralInterface( idDict _data );
	virtual ~blGeneralInterface();

	idDict 	GetWholeData() { return data; };
	void	SetWholeData( idDict _data );

	idStr	GetKeyData( idStr key, idStr defaultValue );
	void	SetKeyData( idStr key, idStr value ) { data.Set( key, value ); };

private:
	void 	Clear();
	void 	ClearData();
	idDict	data;

};

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_INTERFACE_H_ */
