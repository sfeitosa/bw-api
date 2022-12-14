//
// C++ Interface: bwajaxhtmlform
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CGIBWAJAXHTMLFORM_H
#define CGIBWAJAXHTMLFORM_H

#include <bwhttp.h>
#include <bwkeypair.h>
#include <bwlist.h>
#include <bwtemplate.h>
#include <bwconfig.h>
#include <fstream>

#define BW_HTML_MODAL "/srv/beanweb/var/www/html/templates/modal.html"
#define BW_HTML_FORM "/srv/beanweb/var/www/html/templates/form.html"

using namespace std;

namespace CGI
{
  using namespace Http;

  /**
  Classe responsavel por gerenciar formularios genericos, altamente configuraveis e com campos baseados em templates

   @author Samuel da Silva Feitosa
  */
  class BWAjaxHtmlForm
  {
    public:
      BWAjaxHtmlForm(BWKeyPair &, const BWString &file);
      BWAjaxHtmlForm(BWKeyPair &, BWConfig &cfg);

      virtual ~BWAjaxHtmlForm();
      BWKeyPair GetFieldValues();
      BWString operator[](const BWString &idx);
      bool IsFilled();
      bool WasSubmitted();
      void AddField(const BWString &name, const BWString &type, const BWString &label, const BWString &defaultv);
      BWString GetInnerHtml();
      //void SetConfig(const BWString &parm, const BWString &val);
      //void SetFieldParm(BWString &field, const BWString &parm, const BWString &value);

      virtual BWString GetFieldText(const BWString &field)
      {
            return htmlFormGetFieldText(field);
      }

    protected:
      BWConfig m_config;
      BWKeyPair m_data;
      BWList m_fields;
      BWString m_cad;

private:
    BWString htmlFormGetFieldText(const BWString &field);
    void init(BWConfig &cfg);
  };

}

#endif
