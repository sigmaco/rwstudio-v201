

#define ADD_ROLLUP_FUNC         "AddCustomRollup"

#define DEFAULT_ROLLUP_WIDTH    294

class RwExpExportOptions;
class RwExpBspExporter;
class RwExpAnmExporter;
class RwExpDffExporter;
class RwExpSplExporter;
class RwMaxBuilder;

enum ExportType {etDFF, etBSP, etANM, etSPL, etRWS};

typedef int (*RwMaxPluginAddRollupFunc)(IRollupWindow *pRollupWindow,
                                       ExportType type,
                                       int nRollups,
                                       RwExpExportOptions*);



typedef void (*RwMaxPluginSetExportersFunc) (RwExpBspExporter*&     pBspExporter,
                                            RwExpAnmExporter*&   pAnmExporter,
                                            RwExpDffExporter*&   pDffExporter,
                                            RwExpSplExporter*&   pSplExporter);


typedef void (*RwMaxPluginSetBuilderFunc) (RwMaxBuilder*&     pBuilder);

