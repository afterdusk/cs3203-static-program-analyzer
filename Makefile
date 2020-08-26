PLANTUML = plantuml

PKB_FRONTEND = procedure stmt stmtLst Variables
PKB_FRONTEND_PU_FILES = $(PKB_FRONTEND:%=pkb-frontend_%.pu)
PKB_FRONTEND_PNG_FILES = $(PKB_FRONTEND:%=pkb-frontend_%.png)

all: $(PKB_FRONTEND_PNG_FILES)

%.png: %.pu
	$(PLANTUML) $(PKB_FRONTEND_PU_FILES)

clean:
	rm *.png
