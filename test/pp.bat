dsc -f IDL_AllTypes.dsc -c-compact -c-compress -c-LOG
dsc -f IDL_AllTypes2.dsc -c-compact -c-compress -c-LOG
dsc -f IDL_CommHead.dsc -c-compact -c-LOG
dsc -f IDL_BankTransaction.dsc -c-compress -c-LOG
dsc -f IDL_BankXmlTransaction.dsc -c-xml -c-xml-compact -c-LOG
dsc -f IDL_BankJsonTransaction.dsc -c-json -c-json-compact -c-LOG
dsc -f IDL_testable.dsc -sql-pgsql -ec-pgsql -c-LOG
pause
