#ifndef ASINTERNALS_H_HEADER_INCLUDED
#define ASINTERNALS_H_HEADER_INCLUDED

#include "globals.h"
#include "menus.h"

#ifdef SHAPE
#include <X11/extensions/shape.h>
#include <X11/Xresource.h>
#endif /* SHAPE */

struct FunctionData;
struct ASWindow;
struct ASRectangle;
struct ASDatabase;
struct ASWMProps;
struct MoveResizeData;
struct MenuItem;

/**************************************************************************/
/* Global variables :                                                     */
/**************************************************************************/

#ifdef SHAPE
extern int    ShapeEventBase, ShapeErrorBase;
#endif

extern ASFlagType    AfterStepState;              /* see ASS_ flags above */
/* this are linked lists of structures : */
extern struct ASDatabase *Database;
extern ASHashTable       *ComplexFunctions;

/* these hold base config - differennt PATH vars: */
extern char         *PixmapPath;
extern char         *CursorPath;
extern char         *IconPath;
extern char         *ModulePath;

extern int           fd_width, x_fd;

extern struct ASWindow *ColormapWin;

extern struct ASVector *Modules;               /* dynamically resizable array of module_t data structures */
#define MODULES_LIST    VECTOR_HEAD(module_t,*Modules)
#define MODULES_NUM     VECTOR_USED(*Modules)

extern int           Module_fd;
extern int           Module_npipes;

extern Bool   		 menu_event_mask[LASTEvent];  /* menu event filter */

extern int    menuFromFrameOrWindowOrTitlebar;

#ifdef SHAPE
extern int    ShapeEventBase;
#endif /* SHAPE */

/**************************************************************************/
/**************************************************************************/
/* Function prototypes :                                                  */
/**************************************************************************/

/*************************** colormaps.c : ********************************/
void ColormapSetup();
void ColormapCleanup();
void InstallWindowColormaps (ASWindow *asw);
void UninstallWindowColormaps (ASWindow *asw);
void InstallRootColormap (void);
void UninstallRootColormap (void);
void InstallAfterStepColormap (void);
void UninstallAfterStepColormap (void);


/*************************** configure.c **********************************/
void
LoadASConfig (const char *display_name, int thisdesktop, Bool parse_menu,
              Bool parse_look, Bool parse_feel);

/*************************** decorations.c ********************************/
int check_allowed_function2 (int func, ASWindow * t);
int check_allowed_function (struct MenuItem * mi);

/*************************** events.c ********************************/
void DigestEvent    ( ASEvent *event );
void DispatchEvent  ( ASEvent *event );
void HandleEvents   ();
void WaitForButtonsUpLoop ();
Bool WaitEventLoop( ASEvent *event, int finish_event_type, long timeout );
Bool IsClickLoop( ASEvent *event, unsigned int end_mask, unsigned int click_time );
Bool WaitWindowLoop( char *pattern, long timeout );


Bool KeyboardShortcuts (XEvent * xevent, int return_event, int move_size);

extern void HandleFocusIn (struct ASEvent *event);
extern void HandleFocusOut (struct ASEvent *event);
extern void HandleDestroyNotify (struct ASEvent *event);
extern void HandleMapRequest (struct ASEvent *event);
extern void HandleMapNotify (struct ASEvent *event);
extern void HandleUnmapNotify (struct ASEvent *event);
extern void HandleButtonPress (struct ASEvent *event);
extern void HandleEnterNotify (struct ASEvent *event);
extern void HandleLeaveNotify (struct ASEvent *event);
extern void HandleConfigureRequest (struct ASEvent *event);
extern void HandleClientMessage (struct ASEvent *event);
extern void HandlePropertyNotify (struct ASEvent *event);
extern void HandleKeyPress (struct ASEvent *event);
extern void HandleVisibilityNotify (struct ASEvent *event);
extern void HandleColormapNotify (struct ASEvent *event);

#ifdef SHAPE
void          HandleShapeNotify (struct ASEvent *event);
#endif /* SHAPE */

/*************************** functions.c **********************************/
void SetupFunctionHandlers();

void ExecuteFunction (struct FunctionData *data, struct ASEvent *event, int Module);
int  DeferExecution (struct ASEvent *event, int cursor, int FinishEvent);
void QuickRestart (char *what);

/************************* housekeeping.c ********************************/
Bool GrabEm   ( struct ScreenInfo *scr, Cursor cursor );
void UngrabEm ();
Bool StartWarping(struct ScreenInfo *scr, Cursor cursor);
void EndWarping();

void PasteSelection (struct ScreenInfo *scr );

/*************************** menus.c *********************************/
/***************************** module.c ***********************************/
void SetupModules(void);

void ExecModule (char *action, Window win, int context);
int  AcceptModuleConnection (int socket_fd);

void SendPacket (int channel, unsigned long  msg_type, unsigned long num_datum, ...);
void SendString (int channel, unsigned long  msg_type, unsigned long id, unsigned long tag, char *string );
void SendVector (int channel, unsigned long  msg_type, struct ASVector *vector);
void SendConfig (int module, unsigned long event_type, ASWindow * t);
void SendName (int module, unsigned long event_type,
               unsigned long data1, unsigned long data2, unsigned long data3, char *name);

void BroadcastConfig (unsigned long, ASWindow *);
void SendName (int, unsigned long, unsigned long, unsigned long, unsigned long, char *);
/* simplified specialized interface to above functions : */
void broadcast_focus_change( ASWindow *focused );
void broadcast_window_name( ASWindow *asw );
void broadcast_icon_name( ASWindow *asw );
void broadcast_res_names( ASWindow *asw );
void broadcast_status_change( int message, ASWindow *asw );



void HandleModuleInOut(unsigned int channel, Bool has_input, Bool has_output);

void KillModuleByName (char *name);
void DeadPipe (int nonsense);
void ShutdownModules();



/******************************* outline.c ********************************/
void MoveOutline( struct MoveResizeData * pdata );

/******************************* pager.c ***********************************/
/* we use 4 windows that are InputOnly and therefore are invisible on the
 * sides of the screen to steal mouse events and allow for virtual viewport
 * move when cursor reaches edge of the screen. :*/
void MoveViewport (int newx, int newy, Bool grab);
void HandlePaging (int HorWarpSize, int VertWarpSize, int *xl,
                   int *yt, int *delta_x, int *delta_y, Bool Grab, struct ASEvent *event);
void ChangeDesks (int new_desk);
void RaisePanFrames (void);
void CheckPanFrames (void);
void InitPanFrames ();




#endif /* ASINTERNALS_H_HEADER_INCLUDED */
