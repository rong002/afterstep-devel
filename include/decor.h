#ifndef DECOR_H_HEADER
#define DECOR_H_HEADER
#ifndef NO_TEXTURE		/* NO_TEXTURE == No Frames! */

#if !defined(DEBUG_ALLOCS) && defined(DEBUG_TRACE)
#define TRACE_update_canvas_display
#define TRACE_render_astbar
#endif


struct MyStyle ;
struct ASImage;
struct icon_t;
struct button_t;
struct ASImageManager;

typedef struct ASCanvas
{
#define CANVAS_DIRTY				(0x01<<0)
#define CANVAS_OUT_OF_SYNC			(0x01<<1)
#define CANVAS_MASK_OUT_OF_SYNC		(0x01<<2)

/* these are not really a states but rather flags : */
#define CANVAS_CONTAINER            (0x01<<16) /* user drawn - should not maintain our canvas Pixmap,
                                                * also should monitor chnages in user defined shape,
                                                * and update mask pixmap accordingly */
    ASFlagType  state ;
	Window w;
	int root_x, root_y;
	unsigned int width, height ;

	Pixmap canvas;
	Pixmap mask;
    /* 32 bytes */
}ASCanvas;

typedef struct ASTBtnData{
    struct ASImage *unpressed ;
    struct ASImage *pressed ;
    struct ASImage *current ;
    unsigned short width, height ;
    short x, y ;                               /* relative to the button block origin !*/
    unsigned long context ;
    /* 20 bytes */
}ASTBtnData;

#define TBTN_ORDER_L2R      0
#define TBTN_ORDER_T2B      1
#define TBTN_ORDER_R2L      2
#define TBTN_ORDER_B2T      3
#define TBTN_ORDER_VERTICAL (0x01<<0)
#define TBTN_ORDER_REVERSE  (0x01<<1)
#define TBTN_ORDER_MASK     (TBTN_ORDER_VERTICAL|TBTN_ORDER_REVERSE)

typedef struct ASBtnBlock {
    ASTBtnData      *buttons;                  /* array of [count] structures */
    unsigned int     buttons_num;
    /* 8 bytes */
}ASBtnBlock;

typedef struct ASLabel {
    char            *text ;
    struct ASImage  *rendered[2] ;
	/* 12 bytes */
}ASLabel;

typedef struct ASTile {
#define AS_TileSpacer	 	0
#define AS_TileBtnBlock 	1
#define AS_TileIcon		 	2
#define AS_TileLabel	 	3
#define AS_TileFreed        7

#define AS_TileTypeMask     (0x07<<0)
#define AS_TileTypes        8
#define ASTileType(t)      ((t).flags&AS_TileTypeMask)
#define ASSetTileType(tl,ty)  ((tl)->flags=((tl)->flags&(~AS_TileTypeMask))|(ty))
#define ASGetTileType(tl)   (((tl)->flags)&AS_TileTypeMask)

#define AS_TileColOffset    4
#define AS_TileColumns      8
#define AS_TileColMask      (0x07<<AS_TileColOffset)
#define ASTileCol(t)        (((t).flags&AS_TileColMask)>>AS_TileColOffset)

#define AS_TileRowOffset    (AS_TileColOffset+3)
#define AS_TileRows         8
#define AS_TileRowMask      (0x07<<AS_TileRowOffset)
#define ASTileRow(t)        (((t).flags&AS_TileRowMask)>>AS_TileRowOffset)

#define AS_TileFlipOffset   (AS_TileRowOffset+3)
#define AS_TileVFlip        (0x01<<AS_TileFlipOffset)
#define AS_TileUFlip        (0x01<<(AS_TileFlipOffset+1))
#define AS_TileFlipMask     (AS_TileVFlip|AS_TileUFlip)
#define ASTileFlip(t)       (((t).flags&AS_TileFlipMask)>>AS_TileFlipOffset)

#define AS_TileSublayersOffset  (AS_TileFlipOffset+2)
#define AS_TileSublayersMask    (0x00FF<<AS_TileSublayersOffset)  /* max 256 sublayers */
#define ASTileSublayers(t)      (((t).flags&AS_TileSublayersMask)>>AS_TileSublayersOffset)
#define ASSetTileSublayers(t,c)  ((t).flags = ((t).flags&(~AS_TileSublayersMask))|((c<<AS_TileSublayersOffset)&AS_TileSublayersMask))

#define AS_TilePadOffset    (AS_TileSublayersOffset+8)
#define AS_TilePadLeft      (0x01<<AS_TilePadOffset)
#define AS_TilePadRight     (0x01<<(AS_TilePadOffset+1))
#define AS_TileHPadMask     (0x03<<AS_TilePadOffset)
#define AS_TilePadBottom    (0x01<<(AS_TilePadOffset+2))  /* rotated with Top on purpose */
#define AS_TilePadTop       (0x01<<(AS_TilePadOffset+3))
#define AS_TileVPadMask     (0x03<<(AS_TilePadOffset+2))
#define AS_TilePadMask      (AS_TileVPadMask|AS_TileHPadMask)

	ASFlagType flags;
    short x, y;
    unsigned short width, height;
	union {
		ASBtnBlock	 bblock;
        struct ASImage     *icon ;
		ASLabel      label ;
        unsigned long raw[3];
	}data;
    /* 24 bytes */
}ASTile;

typedef struct ASTBarData {
#define BAR_STATE_UNFOCUSED		0
#define BAR_STATE_FOCUSED		(0x01<<0)
#define BAR_STATE_NUM			2
#define BAR_STATE_FOCUS_MASK	(0x01<<0)
#define BAR_STATE_PRESSED		(0x01<<1)
#define BAR_STATE_PRESSED_MASK	(0x01<<1)

#define BAR_FLAGS_REND_PENDING  (0x01<<16)     /* has been moved, resized or otherwise changed and needs rerendering */
#define DoesBarNeedsRendering(pb) get_flags((pb)->state, BAR_FLAGS_REND_PENDING )
#define SetBarNeedsRendering(pb)  set_flags((pb)->state, BAR_FLAGS_REND_PENDING )
#define BAR_FLAGS_VERTICAL      (0x01<<17)     /* vertical label */
#define BAR_FLAGS_IMAGE_BACK    (0x01<<18)     /* back represents an icon instead of  */

#define BAR_FLAGS_HILITE_OFFSET 24
#define BAR_FLAGS_HILITE_MASK   (HILITE_MASK<<BAR_FLAGS_HILITE_OFFSET)
#define BAR_FLAGS2HILITE(f)     (((f)&BAR_FLAGS_HILITE_MASK)>>BAR_FLAGS_HILITE_OFFSET)
#define HILITE2BAR_FLAGS(h)     (((h)<<BAR_FLAGS_HILITE_OFFSET)&BAR_FLAGS_HILITE_MASK)

    ASFlagType  state ;
    unsigned long context ;
    short win_x, win_y ;
    short root_x, root_y;
    /* 16 bytes */
    short rendered_root_x, rendered_root_y;
    unsigned short width, height ;
    unsigned char left_bevel, top_bevel, right_bevel, bottom_bevel ;
    /* 28 bytes */
    /* this is what we make our background from :*/
    struct MyStyle      *style[2] ;
    struct ASImage      *back_image ;
    unsigned short       back_width, back_height;
    /* this is the actuall generated background : */
    struct ASImage      *back [2] ;
    /* 52 bytes */
    unsigned char h_border, v_border;
	unsigned char h_spacing, v_spacing;
	/* 56 bytes */
	ASTile *tiles;
	unsigned int tiles_num ;
	/* 64 bytes */
}ASTBarData ;

/*********************************************************************
 * Window decorations Frame can be defined as such :
 *
 * MyFrame "name"
 *     [Inherit     "name"]
 * #traditional form :
 *     [FrameN   <pixmap>]
 *     [FrameE   <pixmap>]
 *     [FrameS   <pixmap>]
 *     [FrameW   <pixmap>]
 *     [FrameNE  <pixmap>]
 *     [FrameNW  <pixmap>]
 *     [FrameSE  <pixmap>]
 *     [FrameSW  <pixmap>]
 * #alternative form :
 *     [Side        North|South|East|West|Any [<pixmap>]] - if pixmap is ommited -
 *                                                          empty bevel will be drawn
 *     [NoSide      North|South|East|West|Any]
 *     [Corner      NorthEast|SouthEast|NorthWest|SouthWest|Any <pixmap>] - if pixmap is ommited -
 *                                                                          empty bevel will be drawn
 *     [NoCorner    NorthEast|SouthEast|NorthWest|SouthWest|Any]
 *     [SideSize    North|South|East|West|Any <WIDTHxLENGTH>] - pixmap will be scaled to this size
 *     [CornerSize  NorthEast|SouthEast|NorthWest|SouthWest|Any <WIDTHxHEIGHT>]
 * ~MyFrame
 */
typedef struct MyFrame
{
#define MAGIC_MYFRAME           0xA351F385

    unsigned long magic ;
    char      *name;
    ASFlagType flags; /* first 8 bits represent one enabled side/corner each */
    struct icon_t    *parts[FRAME_PARTS];
    char             *part_filenames[FRAME_PARTS];
    unsigned int part_width[FRAME_PARTS];
    unsigned int part_length[FRAME_PARTS];
#define MYFRAME_HOR_MASK    ((0x01<<FR_N)|(0x01<<FR_S))
#define MYFRAME_VERT_MASK   ((0x01<<FR_W)|(0x01<<FR_E))
#define IsSideVertical(side)  ((side) == FR_W || (side)== FR_E)
#define IsFrameCorner(p)   ((p)>=FRAME_SIDES)
#define IsFramePart(f,p)   ((f)->parts[(p)] || ((f)->part_width[(p)] && (f)->part_length[(p)]))

    unsigned int spacing ;
}MyFrame;

extern int    _as_frame_corner_xref[FRAME_SIDES+1];
#define LeftCorner(side)    _as_frame_corner_xref[(side)]
#define RightCorner(side)   _as_frame_corner_xref[(side)+1]

ASCanvas* create_ascanvas(Window w);
ASCanvas* create_ascanvas_container (Window w);

void destroy_ascanvas( ASCanvas **pcanvas );
#define CANVAS_X_CHANGED            (0x01<<0)
#define CANVAS_Y_CHANGED            (0x01<<1)
#define CANVAS_MOVED                (CANVAS_X_CHANGED|CANVAS_Y_CHANGED)
#define CANVAS_WIDTH_CHANGED        (0x01<<2)
#define CANVAS_HEIGHT_CHANGED       (0x01<<3)
#define CANVAS_RESIZED              (CANVAS_WIDTH_CHANGED|CANVAS_HEIGHT_CHANGED)

ASFlagType handle_canvas_config( ASCanvas *canvas ); /* Returns True if moved/resized */
Pixmap get_canvas_canvas( ASCanvas *pc );
Pixmap get_canvas_mask( ASCanvas *pc );
Bool draw_canvas_image( ASCanvas *pc, struct ASImage *im, int x, int y );


#ifdef TRACE_update_canvas_display
void  trace_update_canvas_display (ASCanvas * pc, const char *file, int line);
#define update_canvas_display(p)  trace_update_canvas_display((p),__FILE__,__LINE__)
#else
void update_canvas_display( ASCanvas *pc );
#endif

void resize_canvas( ASCanvas *pc, unsigned int width, unsigned int height );
void move_canvas (ASCanvas * pc, int x, int y);
void moveresize_canvas (ASCanvas * pc, int x, int y, unsigned int width, unsigned int height);
void map_canvas_window( ASCanvas *pc, Bool raised );
void unmap_canvas_window( ASCanvas *pc );
Bool is_canvas_needs_redraw( ASCanvas *pc );
Bool is_canvas_dirty( ASCanvas *pc );



ASTBtnData *create_astbtn();
void        set_tbtn_images( ASTBtnData* btn, struct button_t *from );
ASTBtnData *make_tbtn( struct button_t *from );
void        destroy_astbtn(ASTBtnData **ptbtn );

ASTBarData* create_astbar();
void destroy_astbar( ASTBarData **ptbar );
unsigned int get_astbar_label_width( ASTBarData *tbar );
unsigned int get_astbar_label_height( ASTBarData *tbar );
unsigned int calculate_astbar_height( ASTBarData *tbar );
unsigned int calculate_astbar_width( ASTBarData *tbar );


Bool set_astbar_size( ASTBarData *tbar, unsigned int width, unsigned int height );
Bool set_astbar_hilite( ASTBarData *tbar, ASFlagType hilite );
Bool set_astbar_style_ptr (ASTBarData * tbar, unsigned int state, struct MyStyle *style);
Bool set_astbar_style( ASTBarData *tbar, unsigned int state, const char *style_name );
Bool set_astbar_image( ASTBarData *tbar, struct ASImage *image );
Bool set_astbar_back_size( ASTBarData *tbar, unsigned short width, unsigned short height );


int add_astbar_spacer( ASTBarData *tbar, unsigned char col, unsigned char row, int flip, int align, unsigned short width, unsigned short height);
int add_astbar_btnblock( ASTBarData * tbar, unsigned char col, unsigned char row, int flip, int align,
                     struct button_t *from_list, ASFlagType mask, unsigned int count,
                     int left_margin, int top_margin, int spacing, int order,
                     unsigned long context_base);
int add_astbar_icon( ASTBarData * tbar, unsigned char col, unsigned char row, int flip, int align, struct ASImage *icon);
int add_astbar_label( ASTBarData * tbar, unsigned char col, unsigned char row, int flip, int align, const char *text);
Bool delete_astbar_tile( ASTBarData *tbar, int idx );

Bool change_astbar_label (ASTBarData * tbar, int index, const char *label);
Bool change_astbar_first_label (ASTBarData * tbar, const char *label);


Bool move_astbar( ASTBarData *tbar, ASCanvas *pc, int win_x, int win_y );
Bool set_astbar_focused( ASTBarData *tbar, ASCanvas *pc, Bool focused );
Bool set_astbar_pressed( ASTBarData *tbar, ASCanvas *pc, Bool pressed );
Bool set_astbar_btn_pressed( ASTBarData * tbar, int context );

Bool update_astbar_transparency( ASTBarData *tbar, ASCanvas *pc );
int  check_astbar_point( ASTBarData *tbar, int root_x, int root_y );

#ifdef TRACE_render_astbar
Bool  trace_render_astbar (ASTBarData * tbar, ASCanvas * pc, const char *file, int line);
#define render_astbar(t,p)  trace_render_astbar ((t),(p),__FILE__,__LINE__)
#else
Bool render_astbar( ASTBarData *tbar, ASCanvas *pc );
#endif



MyFrame *create_myframe();
MyFrame *create_default_myframe();
MyFrame *myframe_find( const char *name );
void myframe_load ( MyFrame * frame, struct ASImageManager *imman );
Bool filename2myframe_part (MyFrame *frame, int part, char *filename);
Bool myframe_has_parts(const MyFrame *frame, ASFlagType mask);
void destroy_myframe( MyFrame **pframe );


#endif /* !NO_TEXTURE */
#endif /* DECOR_H_HEADER */
