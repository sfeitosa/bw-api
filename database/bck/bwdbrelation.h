//
// C++ Interface: bwdbrelation
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWDBRELATION_H
#define BWDBRELATION_H

#include <bwlist.h>
#include <bwkeylist.h>
#include <bwdb.h>
#include <bwutils.h>
#include <bwerror.h>

/**
Classe responsavel por manipular relacoes em tabelas do banco de dados.

 @author Samuel da Silva Feitosa
*/
class BWDbRelation {
    public:
        BWDbRelation(BWDb *, const BWString &,
                     const BWString &, const BWString &,
                     bool invert = false);
        ~BWDbRelation();

        void SetFields(const BWString &, const BWString &);
        void SetTables(const BWString &, const BWString &);
        void SetKeepSync();

        void Save();

        void Add(BWList &);
        void AddValue(const BWString &);

        BWString Get(bool left = false);
        BWKeyList GetInfo(BWList &);

        void Load(const BWString &lid = "", const BWString &rid = "");
        void LoadL(const BWString &id = "");
        void LoadR(const BWString &id = "");

        void Clean();
        
    private:
        void emptyItens();
        void deleteItens(BWList &);
        void insertItens(BWList &);

        BWList getDeleteList();
        BWList getInsertList();

        BWDb *m_db;
        BWString m_id;
        BWString m_from;
        BWString m_to;
        bool m_invert;
        bool m_keepsync;


        BWString m_table;
        BWString m_lfield;
        BWString m_rfield;
        BWString m_ltable;
        BWString m_rtable;
        BWString m_orderby;
        BWList m_cur_values;
        BWList m_new_values;
};

#endif
