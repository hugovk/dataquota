
@echo ============== SELF-SIGNED ==========================

@move    DataQuota.sis  DataQuota.bak.sis
@move    DataQuota.sisx DataQuota.bak.sisx
makesis DataQuota.pkg  DataQuota.sis
signsis DataQuota.sis  DataQuota.sisx DataQuota.cer DataQuota.key password

copy DataQuota.sisx "%HOMEDRIVE%%HOMEPATH%\My Documents\Magic Briefcase"
if exist DataQuota.sisx start DataQuota.sisx


@echo ============== OVI SIGNED ==========================

@move    DataQuota_signed.sis  DataQuota_signed.bak.sis
@move    DataQuota_signed.sisx DataQuota_signed.bak.sisx
makesis DataQuota_signed.pkg  DataQuota_signed.sis
signsis DataQuota_signed.sis  DataQuota_signed.sisx ovi_2011-05-30.crt ovi_2011-05-30.key

copy DataQuota_signed.sisx "%HOMEDRIVE%%HOMEPATH%\My Documents\Magic Briefcase"
if exist DataQuota_signed.sisx start DataQuota_signed.sisx
