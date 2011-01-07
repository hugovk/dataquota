move    DataQuota.sis  DataQuota.bak.sis
move    DataQuota.sisx DataQuota.bak.sisx
makesis DataQuota.pkg  DataQuota.sis
signsis DataQuota.sis  DataQuota.sisx DataQuota.cer DataQuota.key password
copy DataQuota.sisx "%HOMEDRIVE%%HOMEPATH%\My Documents\Magic Briefcase"
start DataQuota.sisx
