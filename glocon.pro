TEMPLATE = subdirs

SUBDIRS = \
  src \
  tst \

# Create the build directory structure
system( \
  $${QMAKE_MKDIR} $$shell_path($${OUT_PWD}/build); \
  $${QMAKE_MKDIR} $$shell_path($${OUT_PWD}/build/moc); \
  $${QMAKE_MKDIR} $$shell_path($${OUT_PWD}/build/obj); \
  $${QMAKE_MKDIR} $$shell_path($${OUT_PWD}/build/rcc); \
)
