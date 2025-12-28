ROSTER(3, "Default/New", "F0/F1/F2/F3/F4/F5/F6/F7")
ROSTER(10, "V100/BR112", "Licht/Rot R/Rot F//Rangiergang")
ROSTER(11, "V60/BR106", "Licht////Rangiergang")
ROSTER(12, "V180/BR118", "Licht////Rangiergang")

ALIAS(W_1, 101)
ALIAS(W_2, 102)
ALIAS(W_3, 103)
ALIAS(W_4, 104)
ALIAS(W_5, 105)
ALIAS(W_6, 106)
ALIAS(W_7, 107)
ALIAS(W_8, 108)
ALIAS(W_9, 109)
ALIAS(W_10, 110)
ALIAS(W_11, 111)
ALIAS(W_12, 112)

TURNOUTL(W_1, W_1, "W1 Einfahrt Bhf Ost")
TURNOUTL(W_2, W_2, "W2 Zufahrt Hafen")
TURNOUTL(W_3, W_3, "W3 Zufahrt Lokschuppen")
TURNOUTL(W_4, W_4, "W4 Tunnel EinfahrtWest")
TURNOUTL(W_5, W_5, "W5 Hafen Ost/Nord")
TURNOUTL(W_6, W_6, "W6 Hafen West/Nord")
TURNOUTL(W_7, W_7, "W7 DKW Hafen West")
TURNOUTL(W_8, W_8, "W8 DKW Hafen Ost")
TURNOUTL(W_9, W_9, "W9 Hafen Ost Süd")
TURNOUTL(W_10, W_10, "W10 Tunnel Nord Innen")
TURNOUTL(W_11, W_11, "W11 Tunnel Nord Aussen")
TURNOUTL(W_12, W_12, "W12 Tunnel Nord Auffahrt")

#include "myAlias.h"

AUTOSTART
POWERON
DELAY(1000)
CALL(303) // Alle Signale auf Halt
// START(300)
IF(GBM_G2)
    ROUTE_INACTIVE(B_2_ROUND)
ELSE
    ROUTE_DISABLED(B_2_ROUND)
ENDIF
RETURN

SEQUENCE(303)
    // Alle Signale auf Halt
    DEACTIVATEL(SIG_SP_2)
    DEACTIVATEL(SIG_SP_3)
    DEACTIVATEL(SIG_HS_5)
    DEACTIVATEL(SIG_VS_5)
RETURN

ROUTE(301, "Pause")
PAUSE
DONE

ROUTE(302, "Resume")
    RESUME
DONE

ROUTE(304, "Stop all tasks")
    KILLALL
    IFNOT(GBM_G2)
        FREE(B_2)
    ENDIF
    IFNOT(GBM_G4)
        FREE(B_4)
    ENDIF
    IFNOT(GBM_G1)
        FREE(B_1)
    ENDIF
    IFNOT(GBM_G3)
        FREE(B_3)
    ENDIF
    IFNOT(GBM_G5)
        FREE(B_5)
    ENDIF
    IFNOT(GBM_G6)
        FREE(B_6)
    ENDIF
    IFNOT(GBM_G7)
        FREE(B_7)
    ENDIF
    IFNOT(GBM_G8)
        FREE(B_8)
    ENDIF
DONE

ONTHROW(W_1)
    PRINT("W1 thrown")
    CALL(SIG_SP_2_TEST)
DONE

ONCLOSE(W_1)
    PRINT("W1 closed")
    CALL(SIG_SP_2_TEST)
DONE

ONTHROW(W_2)
    PRINT("W2 thrown")
    CALL(SIG_SP_2_TEST)
DONE

ONCLOSE(W_2)
    PRINT("W2 closed")
    CALL(SIG_SP_2_TEST)
DONE

SEQUENCE(SIG_SP_2_TEST)
    IFTHROWN(W_1)
        IFCLOSED(W_2)
            ACTIVATEL(SIG_SP_2)
        ELSE
            DEACTIVATEL(SIG_SP_2)
        ENDIF
    ELSE
        DEACTIVATEL(SIG_SP_2)
    ENDIF
RETURN

ONTHROW(W_3)
    PRINT("W3 thrown")
    DEACTIVATEL(SIG_SP_3)
    UNJOIN
    LATCH(PROG_TEST)
DONE

ONCLOSE(W_3)
    PRINT("W3 closed")
    JOIN
    UNLATCH(PROG_TEST)
DONE

// Drive away a loco from the programming track: 989, 990, 991, 992, 993
ROUTE(989, "Drive away loco from programming track to B2")
    THROW(W_3)
    ROUTE_INACTIVE(989)
    FOLLOW(990)
DONE

SEQUENCE(990)
    IF(PROG_TEST)
        PRINT("Ready to wait for loco on prog track")
        UNJOIN
        DELAY(2000)
        // Wait for loco
        FOLLOW(991)
    ELSE
        JOIN
        DELAY(1000)
        FOLLOW(990)
    ENDIF
DONE

SEQUENCE(991)
    IF(PROG_TEST)
        READ_LOCO
        IFLOCO(-1) // failed read
            DELAY(5000)
            FOLLOW(991)
        ENDIF
        PRINT("Loco on prog track")
        ACTIVATEL(SIG_SP_3)
        DELAY(1000)
        DEACTIVATEL(SIG_SP_3)
        DELAY(1000)
        ACTIVATEL(SIG_SP_3)
        DELAY(1000)
        DEACTIVATEL(SIG_SP_3)
        DELAY(1000)
        FOLLOW(992)
    ELSE
        FOLLOW(990)
    ENDIF
DONE

SEQUENCE(992)
    PRINT("Driving loco off programming track")
    RESERVE(B_4)
    LATCH(VON_PROG)
    CLOSE(W_3)
    ACTIVATEL(SIG_SP_3)
    DELAYRANDOM(1500, 2500)
    REV(0)
    DELAY(500)
    // Licht an
    FON(0)
    DELAYRANDOM(500, 1000)
    REV(50)
    START_SEND(B_4_VON_PROG)
    DELAY(5000)
    DEACTIVATEL(SIG_SP_3)
    AFTER(VON_PROG)
    PRINT("Loco left prog track")
    ROUTE_ACTIVE(989)
DONE
    
SEQUENCE(B_4_VON_PROG)
    AT(GBM_G4)
    PRINT("B_4: At G4")
    DELAY(500)
    IFRESERVE(B_4)
        RESERVE(B_4)
        PRINT("B_4: Reserved B_4")
    ENDIF
    DELAYRANDOM(2500, 3000)
    STOP
    DELAYRANDOM(1000, 2000)
    PRINT("B_4: Unlatching VON_PROG")
    UNLATCH(VON_PROG)
    DELAYRANDOM(1000, 2500)
    RESERVE(B_2)
    PRINT("B_4: Reserved B_2")
    THROW(W_3)
    DELAYRANDOM(1000, 2500)
    FWD(70)
    FOLLOW(WAIT_AT_B_2)
    //IF(GBM_G4)
    //    AFTER(GBM_G4)
    //ENDIF
    //FREE(B_4)
    //PRINT("B_4: Freed B_4")
DONE

SEQUENCE(B_4)
    AT(GBM_G4)
    PRINT("B_4: At G4")
    IF(FREE_B_7)
        FREE(B_7)
        UNLATCH(FREE_B_7)
        PRINT("B_4: Freed B_7")
    ENDIF
    IF(FREE_B_8)
        FREE(B_8)
        UNLATCH(FREE_B_8)
        PRINT("B_4: Freed B_8")
    ENDIF
    DELAY(500)
    IFRESERVE(B_4)
        RESERVE(B_4)
        PRINT("B_4: Reserved B_4")
    ENDIF
    IF(GBM_G2)
        RESET(ABC_G4)
    ENDIF
    RESERVE(B_2)
    PRINT("B_4: Reserved B_2")
    THROW(W_3)
    FOLLOW(B_2)
    //IF(GBM_G4)
    //    AFTER(GBM_G4)
    //ENDIF
    //FREE(B_4)
    //PRINT("B_4: Freed B_4")
DONE

SEQUENCE(WAIT_AT_B_2)
    AT(GBM_G2)
    PRINT("WAIT_AT_B_2: At G2")
    FREE(B_4)
    PRINT("WAIT_AT_B_2: Freed B_4")
    IFRESERVE(B_2)
        PRINT("WAIT_AT_B_2: Reserved B_2")
    ENDIF
    PRINT("WAIT_AT_B_2: Loco was driven from programming track. Waiting...")
    RESET(ABC_G2)
    DELAY(3000)
    STOP
    FOFF(0)
    SET(ABC_G2)
    ROUTE_INACTIVE(B_2_ROUND)
    DELAY(3000)
    //ROUTE_ACTIVE(B_2_ROUND)
    //FOLLOW(B_2_ROUND)
DONE

SEQUENCE(B_2)
    AT(GBM_G2)
    PRINT("B_2: At G2")
    FREE(B_4)
    PRINT("B_2: Freed B_4")
    SAVESPEED
    IFRESERVE(B_2)
        PRINT("B_2: Reserved B_2")
    ENDIF
    SLOWDOWN_REL(50)
    DELAYRANDOM(500, 1500)
    IFRANDOM(30)
        RESET(ABC_G2)
        DELAYRANDOM(5000, 15000)
    ENDIF
    RESERVE(B_1)
    PRINT("B_2: Reserved B_1")
    CLOSE(W_1)
    DELAYRANDOM(500, 2000)
    SET(ABC_G2)
    RESTORESPEED
    //IF(GBM_G2)
    //    AFTER(GBM_G2)
    //ENDIF
    //FREE(B_2)
    //PRINT("Freed B_2 by B_2")
    FOLLOW(B_1_VON_BHF)
DONE

AUTOMATION(B_2_ROUND, "B_2 round trip")
    PRINT("B_2: Do a round trip")
    ROUTE_DISABLED(B_2_ROUND)
    RESERVE(B_1)
    PRINT("B_2: Reserved B_1")
    CLOSE(W_1)
    FON(0)
    DELAYRANDOM(500, 2000)
    SET(ABC_G2)
    FWD(70)
    ROUTE_INACTIVE(B_2_ROUND)
    /*
    IF(GBM_G2)
        AFTER(GBM_G2)
    ENDIF
    FREE(B_2)
    PRINT("Freed B_2 by B_2")
    */
    FOLLOW(B_1_VON_BHF)
DONE

AUTOMATION(B_2_ZU_B_3, "B_2 to B_3")
    PRINT("B_2_ZU_B_3: Do a trip to B_3")
    ROUTE_DISABLED(B_2_ZU_B_3)
    RESERVE(B_1)
    PRINT("B_2_ZU_B_3: Reserved B_1")
    CLOSE(W_1)
    FON(0)
    DELAYRANDOM(500, 2000)
    SET(ABC_G2)
    FWD(70)
    AT(GBM_G1)
    PRINT("At G1 from Bhf")
    FREE(B_2)
    PRINT("Freed B_2 by B_2_ZU_B_3")
    IFRESERVE(B_1)
        PRINT("Reserved B_1 by B_2_ZU_B_3")
    ENDIF
    DELAY(500)
    STOP
    RESERVE(B_3)
    PRINT("Reserved B_3 by B_2_ZU_B_3")
    THROW(W_1)
    DELAYRANDOM(500, 2000)
    REV(70)
    AT(GBM_G3)
    PRINT("B_2_ZU_B_3: At G3")
    DELAY(500)
    FREE(B_1)
    PRINT("B_2_ZU_B_3: Freed B_1")
    IFRESERVE(B_3)
        PRINT("B_2_ZU_B_3: Reserved B_3")
    ENDIF
    PRINT("B_2_ZU_B_3: Loco was driven B_2_ZU_B_3. Waiting...")
    RESET(ABC_G3)
    DELAY(3000)
    STOP
    FOFF(0)
    SET(ABC_G3)
    ROUTE_INACTIVE(B_2_ZU_B_3)
DONE

AUTOMATION(B_3_ZU_B_2, "B_3 to B_2")
    PRINT("B_3_ZU_B_2: Do a round trip")
    ROUTE_DISABLED(B_3_ZU_B_2)
    RESERVE(B_1)
    PRINT("B_3_ZU_B_2: Reserved B_1")
    THROW(W_1)
    FON(0)
    DELAYRANDOM(500, 2000)
    FWD(70)
    ROUTE_INACTIVE(B_3_ZU_B_2)
    /*
    IF(GBM_G2)
        AFTER(GBM_G2)
    ENDIF
    FREE(B_2)
    PRINT("Freed B_2 by B_2")
    */
    FOLLOW(B_1_VON_BHF)
DONE

SEQUENCE(B_1_VON_BHF)
    AT(GBM_G1)
    PRINT("At G1 from Bhf")
    FREE(B_2)
    PRINT("Freed B_2 by B_1")
    IFRESERVE(B_1)
        PRINT("Reserved B_1 by B_1_FROM_BHF")
    ENDIF
    IFRESERVE(B_8)
        PRINT("Reserved B_8 by B_1_FROM_BHF")
        FOLLOW(B_8)
    ELSE
        IFRESERVE(B_7)
            PRINT("Reserved B_7 by B_1")
            FOLLOW(B_7)
        ELSE
            RESET(ABC_G9)
            DELAY(7000)
            IFRESERVE(B_7)
                PRINT("Reserved B_7 by B_1_FROM_BHF")
                FOLLOW(B_7)
            ELSE
                PRINT("Reserved B_8 by B_1_FROM_BHF")
                RESERVE(B_8)
                FOLLOW(B_8)
            ENDIF
        ENDIF
    ENDIF
DONE

SEQUENCE(B_8)
    PRINT("B_1_VON_BHF: Driving to B_8")
    CLOSE(W_12)
    THROW(W_11)
    SET(ABC_G9)
    //IF(GBM_G1)
    //    AFTER(GBM_G1)
    //ENDIF
    AT(GBM_G8)
    PRINT("At G8")
    IFRESERVE(B_8)
        PRINT("Reserved B_8 by B_8")
    ENDIF
    FREE(B_1)
    PRINT("Freed B_1 by B_8")
    DELAY(1000)
    RESET(ABC_G8)
    DELAYRANDOM( 5000, 15000)
    RESERVE(B_4)
    PRINT("Reserved B_4 by B_8")
    THROW(W_4)
    SET(ABC_G8)
    LATCH(FREE_B_8)
    /*
    IF(GBM_G8)
        AFTER(GBM_G8)
    ENDIF
    FREE(B_8)
    PRINT("Freed B_8 by B_8")
    */
    FOLLOW(B_4)
DONE

SEQUENCE(B_7)
    PRINT("B_1_VON_BHF: Driving to B_7")
    CLOSE(W_12)
    CLOSE(W_11)
    CLOSE(W_10)
    SET(ABC_G9)
    //IF(GBM_G1)
    //    AFTER(GBM_G1)
    //ENDIF
    AT(GBM_G7)
    PRINT("At G7")
    IFRESERVE(B_7)
        PRINT("Reserved B_7 by B_7")
    ENDIF
    FREE(B_1)
    PRINT("Freed B_1 by B_7")
    RESET(ABC_G7)
    DELAYRANDOM( 5000, 15000)
    RESERVE(B_4)
    PRINT("Reserved B_4 by B_7")
    CLOSE(W_4)
    SET(ABC_G7)
    LATCH(FREE_B_7)
    /*
    IF(GBM_G7)
        AFTER(GBM_G7)
    ENDIF
    FREE(B_7)
    PRINT("Freed B_7 by B_7")
    */
    FOLLOW(B_4)
DONE