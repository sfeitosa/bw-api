//
// C++ Interface: bwdbregistry
//
// Description:
//
//
// Author: Bruno Moreira Guedes <bruno@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BWDBREGISTRY_H
#define BWDBREGISTRY_H

#include <bwlist.h>
#include <bwdb.h>
#include <bwutils.h>

#ifdef __BWDBREGISTRY_CPP

#ifndef __EMPTY_L
#define __EMPTY_L
BWList __dbreg_empty_list;
#endif//__EMPTY_L

#else
extern BWList __dbreg_empty_list;
#endif//__BWDBREGISTRY_CPP

/**
Classe responsavel por gerar os SQLs para adicao, remocao e edicao de registros  do banco, bem como executa-las na base.

 @author Bruno Moreira Guedes
*/
class BWDbRegistry
{
  public:
   BWDbRegistry(const BWDb &db,
                const BWString &dbtable,
                BWList &fields=__dbreg_empty_list,
                BWList &mreltables=__dbreg_empty_list);

    ~BWDbRegistry();
    BWDbRegistry& Create();
    BWDbRegistry& Delete();
    BWDbRegistry& Insert();
    BWDbRegistry& Load(BWKeyPair &kpr);
    BWDbRegistry& Load(int id);
    BWDbRegistry& Save();
    BWDbRegistry& SaveAs(int id);
    BWDbRegistry& SaveAsNew();
    BWDbRegistry& Set(BWKeyPair &fields);
    BWDbRegistry& SetDb(const BWDb &db);
    BWDbRegistry& SetField(const BWString &field, const BWString &v);
    BWDbRegistry& SetRelation(const BWString &field, BWList &value);
    BWDbRegistry& SetRelation(const BWString &field, const BWString &newvalue);
    BWDbRegistry& Update();
    BWString GetTable() const;
    BWString GetField(const BWString &field);
    BWString operator[](const BWString &field);
    int GetId() const;
    BWString GetRelations(const BWString &rel);
    BWDbRegistry& AddField(const BWString &field);
    BWDbRegistry& AddRelation(const BWString &table);
    BWDbRegistry& SetIdField(const BWString &id);
    BWDbRegistry& SetTablePrefix(const BWString &p);
    BWDbRegistry& SetId(int id);
    BWDbRegistry& Clear();
    BWDbRegistry& EmptyRelation(const BWString &relation);
    BWKeyPair GetRelationOptions(const BWString &rel, const BWString &idfield="id", const BWString &descrfield="descr", const BWString &reltab="");
    BWList GetRelationOptionIds(const BWString &relation, const BWString &idfield="id");
    BWList GetRelationOptionDescrs(const BWString &rel, const BWString &descrfield="descr");

    BWKeyPair GetFieldOptions(const BWString &field, const BWString &optidfield="id", const BWString &optdescrfield="descr");
    BWList GetFieldOptionIds(const BWString &field, const BWString &optidfield="id");
    BWList GetFieldOptionDescrs(const BWString &field, const BWString &optdescrfield="descr");

    void SetKeepSync();
    bool DbChanged();

  protected:
    void saveRelation(const BWString &relation);
    void loadRelation(const BWString &rel);
    void delRegistry(const BWString &table, const BWString &idfield, 
                     const BWString &idvalue);
    void delRelations(const BWString &table, BWKeyPair data);

    BWKeyList rel_curr_values;
    BWKeyList rel_new_values;
    BWKeyPair reg_curr_values;
    BWKeyPair reg_new_values;
    BWString reg_id;
    BWDb* bw_db;
    BWString reg_table;
    BWList reg_fields;
    BWString reg_idfield_name;
    BWList reg_relations;
    BWString reg_tabprefix;
    bool bw_regnew;
    BWString reg_tablename;
    bool m_keepsync;
    bool m_dbchanged;
};

#endif
