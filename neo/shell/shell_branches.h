/*
 * shell_banches.h
 *
 *  Created on: 29 12 2019
 *      Author: Biel Bestué de Luna
 */

#ifndef NEO_SHELL_SHELL_BRANCHES_H_
#define NEO_SHELL_SHELL_BRANCHES_H_

#include "../idlib/containers/List.h"
#include "../idlib/Str.h"

namespace BFG {

enum BranchStyle_e
{
	BRANCH_FRAMED,
	BRANCH_PROCESSED,
	NUM_BRANCHSTYLES
};

class blShellBranch {
public:
	virtual				~blShellBranch() {};
	virtual	void		Init() = 0;
	virtual void		Shutdown() = 0;

	virtual void		PreProcess() = 0; //not run every frame but once before the first frame. Prep things up when the process is in pause before it starts to function
	virtual void		PostProcess() = 0; //run after the last frame this could serve as enabling a next state

	idStr				GetBranchId() { return BranchId; }
	void				SetBranchId( idStr newId ) { BranchId = newId; }

	int					GetBranchStyle() { return Style; }

  void        SetContainedBranch( blShellBranch containee ) { containedBranches_l.append( containee ); }
	int					GetContainedBranchesNum() { return containedBranches_l.num; }
	blShellBranch		GetContainedBranch( int index ) { return containedBranches_l[index]; }

  void				SetParentBranch( blShellBranch* parent_ptr ) { ParentBranch = parent_ptr; ParentBranch->SetContainedBranch( *this ); }
	blShellBranch*		GetParentBranch() { return ParentBranch; }

	void				ToggleHidden() { hidden = !hidden; }
	bool				IsHidden() { return hidden; }

private:
	bool				hidden;
	idStr 				BranchId;
	virtual int			Style = 0;

	idList<blShellBranch> containedBranches_l;

	blShellBranch*		ParentBranch;

	// to be called within any Clear() occurs;
	void				Clear_blShellBranch() { ParentBranch = NULL; containedBranches_l.Clear(); }

	// to be called whenever shutdown occurs after any Clear();
	void				ShutDown_blShellBranch() { delete ParentBranch; }

};

class blShellBranchFramed : blShellBranch {
public:
	virtual 			~blShellBranchFramed() {};
	virtual	void		Init() = 0;
	virtual void		Shutdown() = 0;
	virtual	int			Style = BranchStyle_e::BRANCH_FRAMED;
	virtual void		RunFrame() = 0; //run every Frame;
	virtual void		PreProcess() = 0; //not run every frame but once before the first frame. Prep things up when the process is in pause before it starts to function
	virtual void		PostProcess() = 0; //run after the last frame this could serve as enabling a next state
};

enum BranchProcess_e
{
	BRANCH_PROCESS_NOT_STARTED,
	BRANCH_PROCESS_STARTED,
	BRANCH_PROCESS_PAUSED,
	BRANCH_PROCESS_STALLED,
	BRANCH_PROCESS_FINISHED,
	NUM_BRANCH_PROCESS
};

class blShellBranchProcessed : blShellBranch {
public:
	virtual 			~blShellBranchProcessed() {};
	virtual	void		Init() = 0;
	virtual void		Shutdown() = 0;
	virtual	int			Style = BranchStyle_e::BRANCH_PROCESSED;
	virtual int			ProcessStatus = BranchProcess_e::BRANCH_PROCESS_NOT_STARTED;
	virtual void		StartProcess( idList<blShellBranch>* List ) = 0; //run for the duration of the process the thing that needs to be processed
	virtual void		PreProcess() = 0; //not run every frame but once before the first frame. Prep things up when the process is in pause before it starts to function
	virtual void		PostProcess() = 0; //run after the last frame this could serve as enabling a next state

private:
  idList<blShellBranch>* Branches_l_ptr;
};

/*
class blShellBranchEscapeInputs : blShellBranchFramed {
public:
	virtual 			~blShellBranchEscapeInputs() {};
	virtual void		Init();
	virtual void		Shutdown();
	virtual void		PreProcess();
	virtual void		RunFrame();
	virtual void		PostProcess();
	void				AddEscapeInputs( idList<ShellInterruptEvent> Possible_Escape_Inputs_l );

private:
	void				Clear();

	idList<ShellInterruptEvent> Escape_Inputs_l;
};
*/

class blShellBranchLineal : blShellBranchFramed {
public:
	virtual     ~blShellBranchLineal() {};
	virtual void		Init();
	virtual void		Shutdown();
	virtual void		PreProcess();
	virtual void		RunFrame();
	virtual void		PostProcess();
	void				FeedSplashList( ShellSplashStageContent_t newElement ) { splashContent_l.Append( newElement ); }

private:
	void				Clear();
	int					LastFrameTime;
	int					CurrentIndex;

	idList<ShellSplashStageContent_t> splashContent_l;
};

class blShellBranchGui : blShellBranchFramed {
public:
	virtual 			~blShellBranchGui() {};
	virtual void		Init();
	virtual void		Shutdown();
	virtual void		PreProcess();
	virtual void		RunFrame();
	virtual void		PostProcess();
private:
	void				Clear();

	idUserInterface*	guiMainMenu;
	idListGUI*			guiMainMenu_MapList;		// easy map list handling
	//idUserInterface *	guiRestartMenu;
	//idUserInterface *	guiGameOver;
	//idUserInterface *	guiMsg;
	//idUserInterface *	guiTakeNotes;
	//idUserInterface *	guiIntro;

	//idUserInterface *	guiInGame;
	//idUserInterface *	guiTest;
};

class blShellBranchFullScreenMaterial : blShellBranchFramed {
public:
	virtual 			~blShellBranchFullScreenMaterial() {};
	virtual void		Init();
	virtual void		Shutdown();
	virtual void		PreProcess();
	virtual void		RunFrame();
	virtual void		PostProcess();
	void				SetContent( static idMaterial* mat, idVec4 tint, idVec4 backCol, idVec2 pos, float aspectR, idVec2 size, idVec4 percent );
private:
	void				Clear();

	static idMaterial*	Material;
	idVec4				TintColour;
	idVec4				BackroundColour;
	idVec2				Position;
	float				  IntendedAspectRatio;
  idVec2        Size;
	idVec4				TexturePercentages;
};

 /*
struct InputUiCommandPair {
	ShellEvent InputEvent;
	idStr UiCommand;
};

class blShellBranchInput : blShellBranchFramed {
public:
	virtual 			~blShellBranchInput() {};
	virtual void		Init();
	virtual void		Shutdown();
	virtual void		PreProcess();
	virtual void		RunFrame();
	virtual void		PostProcess();
  void        Set_UiCommand( idStr NewUiCommand );
  void        Del_UiCommand( idStr UnwantedUiCommand );

  void        Set_InputEventPairedWithUiCommand( InputUiCommandPair NewPair, bool exclusive = false );
  void        Del_InputEvent( ShellEvent UnwantedEv );

private:
	void				Clear();
  bool        CheckUiCommand( idStr DistinctUiCommand );
  void        flag_UiCommand( idStr DistinctUiCommand );
  void        flag_PairedUiCommands( ShellEvent DistinctInputEvent );
  idList<idStr>   UiCommands_l;
  idList<InputUiCommandPair>  pairs_l;

  idList<idStr>   UiCommandsFlagged_l;
};
*/

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_BRANCHES_H_ */
